#include "Controllers/GameSessionController.hpp"
#include <sstream>
#include <assert.h>
#include <string.h>

using namespace BlackjackLogic;
using namespace BlackjackServer::Controllers;
using namespace BlackjackServer::Models;
using namespace DataLayer;

static const auto notStartedGmState = GameSession::State::kNotStarted;

GameSessionController::GameSessionController(
	const Views::GameSessionViews* views,
	const Services::Logger* logger,
	const Services::SendHelper* sendHelper,
	const Services::UserManager* userManager,
	DataLayer::BjDatabase* dbContext,
	
	const BlackjackLogic::GameStatusLogic* gmStatusLogic,
	const BlackjackLogic::PlayerLogic* playerLogic,
	const BlackjackLogic::DealerLogic* dealerLogic
) :
	m_views(views),
	m_logger(logger),
	m_sendHelper(sendHelper),
	m_userManager(userManager),
	m_dbContext(dbContext),
	m_gmStatusLogic(gmStatusLogic),
	m_playerLogic(playerLogic),
	m_dealerLogic(dealerLogic)
{
}

void GameSessionController::startGame(
	const std::vector<DataLayer::UserModel*>& users) const
{
	m_sendHelper->logAndBroadcast(m_views->startGame_View({ &users }));
	
	m_dbContext->getGameSession().state = GameSession::State::kBettingPhase;
	createThePlayers(users);
	
	m_sendHelper->logAndBroadcast(m_views->startBettingPhase_View());
}

void GameSessionController::startHitStandPhase() const
{
	m_sendHelper->logAndBroadcast(m_views->startHitStandPhase_View());
	
	m_dbContext->getGameSession().state = GameSession::State::kHitStandPhase;
	m_dealerLogic->init();
	m_dealerLogic->dealFirstDealersCards();
	m_dealerLogic->dealPlayersCards();
	
	printInitialCardsSetup();
	
	if (m_gmStatusLogic->allHandsAreStanding())
		startCashing();
}

void GameSessionController::startCashing() const
{
	m_sendHelper->logAndBroadcast(m_views->startCashing_View());
	
	m_dealerLogic->dealFinalDealersCards();
	m_sendHelper->logAndBroadcast(
		m_views->dealersFinalCards_View(
			*m_dbContext->getGameSession().dealersHand,
			m_dbContext->getGameSession().dealersHand->cards.size() - 2));
	
	cashPlayers();
	m_userManager->unreadyAllPlayers();
	endGame();
}

void GameSessionController::endGame() const
{
	m_dbContext->getGameSession().state = GameSession::State::kNotStarted;
	
	m_dealerLogic->endTheGame();
	for (auto& pair : m_dbContext->getPlayers())
	{
		delete pair.second;
	}
	m_dbContext->getPlayers().clear();
	
	const auto view = m_views->resetingTheGame_View();
	m_logger->logAction(view);
	m_sendHelper->broadcastMsg(view);
}

void GameSessionController::leaveGame(
	const SocketConnection::Connection& connection,
	const DataLayer::PlayerModel& player) const
{
	m_dbContext->getPlayers().erase(player.userModel->uniqueId);
	
	if (m_dbContext->getGameSession().state != notStartedGmState &&
		m_dbContext->getPlayers().size() == 0)
	{
		endGame();
		return;
	}
	
	if (m_gmStatusLogic->allPlayersHavePlacedTheirBets())
		startHitStandPhase();
	else if (m_gmStatusLogic->allHandsAreStanding())
		startCashing();
}

/*
** Controller actions.
*/

int GameSessionController::betRequest(
	const SocketConnection::Connection& connection,
	const Requests::Request& request) const
{
	if (!basicRequestValidation(GameSession::State::kBettingPhase, connection))
		return -1;

	auto& player = *m_dbContext->getPlayers()[connection.socket];
	auto bet = DataLayer::Bet();

	assert(request.dataLen == sizeof(bet));
	memcpy(&bet, request.data, sizeof(bet));
	
	if (!tryExecuteBet(connection, player, bet))
		return -1;
	
	const auto model = Models::GmSessionModels::SuccessfulBetModel(
	{
		&player,
		&bet
	});
	m_logger->logAction(connection, m_views->successfulBet_ServerView(model));
	m_sendHelper->broadcastMsg(m_views->successfulBet_View(model));
	
	if (m_gmStatusLogic->allPlayersHavePlacedTheirBets())
		startHitStandPhase();
	
	return 0;
}

int GameSessionController::hitRequest(
	const SocketConnection::Connection& connection,
	const Requests::Request& request) const
{
	if (!basicRequestValidation(GameSession::State::kHitStandPhase, connection))
		return -1;
	
	const auto& user = *m_dbContext->getUser(connection);
	auto& player = *m_dbContext->getPlayers()[user.uniqueId];
	
	const PlayingCards::Card* newCard = tryExecuteHit(connection, player);
	if (newCard == nullptr)
		return -1;
	
	const auto view = m_views->successfullHit_View({ newCard, &player });
	m_logger->logAction(connection, view);
	m_sendHelper->broadcastMsg(view);
	
	if (m_gmStatusLogic->allHandsAreStanding())
		startCashing();
	
	return 0;
}

int GameSessionController::standRequest(
	const SocketConnection::Connection& connection,
	const Requests::Request& request) const
{
	if (!basicRequestValidation(GameSession::State::kHitStandPhase, connection))
		return -1;
	
	const auto& user = *m_dbContext->getUser(connection);
	auto& player = *m_dbContext->getPlayers()[user.uniqueId];
	
	std::string exception("");
	try
	{
		m_playerLogic->executeStand(player, 0);
	}
	catch (PlayerLogic::NoSuchHand)
	{
		exception = m_views->noSuchHand_View(player.hands.size());
	}
	catch (PlayerLogic::HandIsNotSitting)
	{
		exception = m_views->handNotSitting_View(0);
	}
	
	if (!exception.empty())
	{
		m_logger->logAction(connection, "Hit: " + exception);
		m_sendHelper->whisperMsg(connection, exception);
		return -1;
	}
	
	const auto view = m_views->successfullStand({ &player });
	m_logger->logAction(connection, view);
	m_sendHelper->broadcastMsg(view);
	
	if (m_gmStatusLogic->allHandsAreStanding())
		startCashing();
	return 0;
}

/*
** Private methods.
*/

void GameSessionController::createThePlayers(
	const std::vector<DataLayer::UserModel*>& users) const
{
	for (const auto& user : users)
	{
		auto newPlayer = new DataLayer::PlayerModel({ user });
		auto firstHand = new DataLayer::PlayerHand();
		newPlayer->hands.push_back(firstHand);
		
		m_dbContext->getPlayers().insert(
			std::make_pair(user->uniqueId, newPlayer));
	}
}

void GameSessionController::printInitialCardsSetup() const
{
	const auto players = m_userManager->getPlayers();
	const auto view = m_views->initialHandsSetup_View(
	{
		m_dbContext->getGameSession().dealersHand->cards[0],
		&players
	});

	m_logger->logAction(view);
	m_sendHelper->broadcastMsg(view);
}

void GameSessionController::cashPlayers() const
{
	std::stringstream ss;
	const auto dealersPoints = m_dealerLogic->dealersHandPoints();
	
	for (auto& pair : m_dbContext->getPlayers())
	{
		auto& player = *pair.second;
		
		ss << std::endl << player.userModel->name << ":\n";
		for (const auto& hand : player.hands)
		{
			const auto cashResult = m_playerLogic->cash(*hand, dealersPoints);
			
			ss	<< "- " << m_views->handCashResult_View(*hand, cashResult)
				<< std::endl;
			
			player.userModel->money += cashResult.receivedMoney;
			m_logger->logAction(
				m_views->handCashResult_ServerView(player, *hand, cashResult));
		}
	}
	m_sendHelper->broadcastMsg(ss.str());
}

bool GameSessionController::basicRequestValidation(
	const DataLayer::GameSession::State requiredGameState,
	const SocketConnection::Connection& connection) const
{
	std::string exception("");
	
	if (m_dbContext->getGameSession().state == GameSession::State::kNotStarted)
	{
		exception = m_views->noGames_View();
	}
	else if (m_dbContext->getGameSession().state != requiredGameState)
	{
		if (requiredGameState == GameSession::State::kHitStandPhase)
			exception = m_views->notHitStandPhase();
		else if (requiredGameState == GameSession::State::kBettingPhase)
			exception = m_views->notBettingPhase_View();
		else
			throw;
	}
	else if (!m_userManager->userIsAPlayer(*m_dbContext->getUser(connection)))
	{
		exception = m_views->notAPlayer_View();
	}
	
	if (exception.empty())
		return true;
	
	m_sendHelper->whisperMsg(connection, exception);
	m_logger->logAction(connection, exception);
	
	return false;
}


bool GameSessionController::tryExecuteBet(
	const SocketConnection::Connection& connection,
	DataLayer::PlayerModel& player,
	const DataLayer::Bet& bet) const
{
	std::string exception("");
	
	try
	{
		m_playerLogic->executeBet(player, bet);
	}
	catch (PlayerLogic::NoSuchHand)
	{
		exception = m_views->noSuchHand_View(player.hands.size());
	}
	catch (PlayerLogic::HandIsNotSitting)
	{
		exception = m_views->handNotSitting_View(bet.handIndex);
	}
	catch (PlayerLogic::InvalidBet)
	{
		exception = m_views->invalidBet_View(bet);
	}
	catch (PlayerLogic::NotEnoughMoney)
	{
		exception = m_views->notEnoughMoney_View(
			player.userModel->money,
			bet.amount);
	}
	catch (PlayerLogic::AlreadyPlacedABet)
	{
		exception = m_views->alreadyPlacedABet_View(
			player.hands[bet.handIndex]->bet);
	}
	
	if (!exception.empty())
	{
		m_logger->logAction(connection, "Bet: " + exception);
		m_sendHelper->whisperMsg(connection, exception);
		return false;
	}
	return true;
}

const PlayingCards::Card* GameSessionController::tryExecuteHit(
	const SocketConnection::Connection& connection,
	DataLayer::PlayerModel& player) const
{
	std::string exception("");
	const PlayingCards::Card* newCard;
	
	try
	{
		const auto dealCardF = [&]() -> const PlayingCards::Card&
		{
			return m_dealerLogic->dealCard();
		};
		
		newCard = m_playerLogic->executeHit(player, 0, dealCardF);
	}
	catch (PlayerLogic::NoSuchHand)
	{
		exception = m_views->noSuchHand_View(player.hands.size());
	}
	catch (PlayerLogic::HandIsNotSitting)
	{
		exception = m_views->handNotSitting_View(0);
	}
	
	if (!exception.empty())
	{
		m_logger->logAction(connection, "Hit: " + exception);
		m_sendHelper->whisperMsg(connection, exception);
		return nullptr;
	}
	
	return newCard;
}
