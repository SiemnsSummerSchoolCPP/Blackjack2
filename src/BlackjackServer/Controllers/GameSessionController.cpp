#include "Controllers/GameSessionController.hpp"
#include <sstream>

using namespace BlackjackLogic;
using namespace BlackjackServer::Controllers;
using namespace BlackjackServer::Models;
using namespace DataLayer;

GameSessionController::GameSessionController(
	const Views::GameSessionViews& views,
	const Services::Logger& logger,
	const Services::SendHelper& sendHelper,
	const Services::UserManager& userManager,
	DataLayer::BjDatabase& dbContext,
	
	BlackjackLogic::GameStatusLogic& gmStatusLogic,
	BlackjackLogic::PlayerLogic& playerLogic,
	BlackjackLogic::DealerLogic& dealerLogic
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
	const auto view = m_views.startGame_View({ &users });
	m_sendHelper.broadcastMsg(view);
	m_logger.logAction(view);
	
	m_dbContext.getGameSession().state = GameSession::State::kBettingPhase;
	createThePlayers(users);
	
	const auto betPhaseView = m_views.startBettingPhase_View();
	m_sendHelper.broadcastMsg(betPhaseView);
	m_logger.logAction(betPhaseView);
}

void GameSessionController::startHitStandPhase() const
{
	// General announcement.
	{
		const auto view = m_views.startHitStandPhase_View();
		m_logger.logAction(view);
		m_sendHelper.broadcastMsg(view);
	}
	
	m_dbContext.getGameSession().state = GameSession::State::kHitStandPhase;
	m_dealerLogic.init();
	m_dealerLogic.dealFirstDealersCards();
	m_dealerLogic.dealPlayersCards();
	
	// Print the initial cards setup.
	{
		const auto players = m_userManager.getPlayers();
		const auto view = m_views.initialHandsSetup_View(
		{
			m_dbContext.getGameSession().dealersHand->cards[0],
			&players
		});
		
		m_logger.logAction(view);
		m_sendHelper.broadcastMsg(view);
	}
	
	if (m_gmStatusLogic.allHandsAreStanding())
		startCashing();
}

void GameSessionController::startCashing() const
{
	{
		const auto view = m_views.startCashing_View();
		m_logger.logAction(view);
		m_sendHelper.broadcastMsg(view);
	}
	
	{
		m_dealerLogic.dealFinalDealersCards();
		const auto view = m_views.dealersFinalCards_View(
			*m_dbContext.getGameSession().dealersHand,
			m_dbContext.getGameSession().dealersHand->cards.size() - 2);
		
		m_logger.logAction(view);
		m_sendHelper.broadcastMsg(view);
	}
	
	std::stringstream ss;
	
	const auto dealersPoints = m_dealerLogic.dealersHandPoints();
	for (auto& pair : m_dbContext.getPlayers())
	{
		auto& player = *pair.second;
		
		ss << std::endl << player.userModel->name << ":\n";
		for (const auto& hand : player.hands)
		{
			const auto cashResult = m_playerLogic.cash(*hand, dealersPoints);
			
			ss	<< "- " << m_views.handCashResult_View(*hand, cashResult)
				<< std::endl;
			
			player.userModel->money += cashResult.receivedMoney;
			m_logger.logAction(
				m_views.handCashResult_ServerView(player, *hand, cashResult));
		}
	}
	
	m_sendHelper.broadcastMsg(ss.str());
	
	for (auto& pair : m_dbContext.getPlayers())
	{
		pair.second->userModel->joinState.isReady = false;
	}
	
	endGame();
}

void GameSessionController::endGame() const
{
	m_dbContext.getGameSession().state = GameSession::State::kNotStarted;
	
	m_dealerLogic.endTheGame();
	for (auto& pair : m_dbContext.getPlayers())
	{
		delete pair.second;
	}
	m_dbContext.getPlayers().clear();
	
	const auto view = m_views.resetingTheGame_View();
	m_logger.logAction(view);
	m_sendHelper.broadcastMsg(view);
}

void GameSessionController::leaveGame(
	const SocketConnection::Connection& connection,
	const DataLayer::PlayerModel& player)
{
	m_dbContext.getPlayers().erase(player.userModel->uniqueId);
	
	if (m_dbContext.getGameSession().state != GameSession::State::kNotStarted &&
		m_dbContext.getPlayers().size() == 0)
	{
		endGame();
		return;
	}
	
	if (m_gmStatusLogic.allPlayersHavePlacedTheirBets())
		startHitStandPhase();
	else if (m_gmStatusLogic.allHandsAreStanding())
		startCashing();
}

/*
** Controller actions.
*/

int GameSessionController::betRequest(
	const SocketConnection::Connection& connection,
	const Requests::Request& request) const
{
	static const auto noGame = DataLayer::GameSession::State::kNotStarted;
	static const auto betPhase = DataLayer::GameSession::State::kBettingPhase;
	
	if (m_dbContext.getGameSession().state == noGame)
	{
		m_sendHelper.whisperMsg(connection, m_views.noGames_View());
		return -1;
	}
	
	if (m_dbContext.getGameSession().state != betPhase)
	{
		m_sendHelper.whisperMsg(connection, m_views.notBettingPhase_View());
		return -1;
	}
	
	const auto& user = *m_dbContext.getUser(connection);
	if (!m_userManager.userIsAPlayer(user))
	{
		m_sendHelper.whisperMsg(connection, m_views.notAPlayer_View());
		return -1;
	}
	
	auto& player = *m_dbContext.getPlayers()[user.uniqueId];
	auto bet = DataLayer::Bet();

	assert(request.dataLen == sizeof(bet));
	memcpy(&bet, request.data, sizeof(bet));
	
	std::string exception("");
	try
	{
		m_playerLogic.executeBet(player, bet);
	}
	catch (PlayerLogic::NoSuchHand)
	{
		exception = m_views.noSuchHand_View(player.hands.size());
	}
	catch (PlayerLogic::HandIsNotSitting)
	{
		exception = m_views.handNotSitting_View(bet.handIndex);
	}
	catch (PlayerLogic::InvalidBet)
	{
		exception = m_views.invalidBet_View(bet);
	}
	catch (PlayerLogic::NotEnoughMoney)
	{
		exception = m_views.notEnoughMoney_View(user.money, bet.amount);
	}
	catch (PlayerLogic::AlreadyPlacedABet)
	{
		exception = m_views.alreadyPlacedABet_View(
			player.hands[bet.handIndex]->bet);
	}
	
	if (!exception.empty())
	{
		m_logger.logAction(connection, "Bet: " + exception);
		m_sendHelper.whisperMsg(connection, exception);
		return -1;
	}
	
	const auto model = Models::GmSessionModels::SuccessfulBetModel(
	{
		&player,
		&bet
	});
	m_logger.logAction(connection, m_views.successfulBet_ServerView(model));
	m_sendHelper.broadcastMsg(m_views.successfulBet_View(model));
	
	if (m_gmStatusLogic.allPlayersHavePlacedTheirBets())
		startHitStandPhase();
	
	return 0;
}

int GameSessionController::hitRequest(
	const SocketConnection::Connection& connection,
	const Requests::Request& request) const
{
	static const auto noGame = DataLayer::GameSession::State::kNotStarted;
	static const auto hsPhase = DataLayer::GameSession::State::kHitStandPhase;
	
	if (m_dbContext.getGameSession().state == noGame)
	{
		m_sendHelper.whisperMsg(connection, m_views.noGames_View());
		return -1;
	}
	
	if (m_dbContext.getGameSession().state != hsPhase)
	{
		m_sendHelper.whisperMsg(connection, m_views.notHitStandPhase());
		return -1;
	}
	
	const auto& user = *m_dbContext.getUser(connection);
	if (!m_userManager.userIsAPlayer(user))
	{
		m_sendHelper.whisperMsg(connection, m_views.notAPlayer_View());
		return -1;
	}
	
	auto& player = *m_dbContext.getPlayers()[user.uniqueId];
	const PlayingCards::Card* newCard;
	
	std::string exception("");
	try
	{
		const auto dealCardF = [&]() -> const PlayingCards::Card&
		{
			return m_dealerLogic.dealCard();
		};
		
		newCard = m_playerLogic.executeHit(player, 0, dealCardF);
	}
	catch (PlayerLogic::NoSuchHand)
	{
		exception = m_views.noSuchHand_View(player.hands.size());
	}
	catch (PlayerLogic::HandIsNotSitting)
	{
		exception = m_views.handNotSitting_View(0);
	}
	
	if (!exception.empty())
	{
		m_logger.logAction(connection, "Hit: " + exception);
		m_sendHelper.whisperMsg(connection, exception);
		return -1;
	}
	
	const auto view = m_views.successfullHit_View({ newCard, &player });
	m_logger.logAction(connection, view);
	m_sendHelper.broadcastMsg(view);
	
	if (m_gmStatusLogic.allHandsAreStanding())
		startCashing();
	
	return 0;
}

int GameSessionController::standRequest(
	const SocketConnection::Connection& connection,
	const Requests::Request& request) const
{
	static const auto noGame = DataLayer::GameSession::State::kNotStarted;
	static const auto hsPhase = DataLayer::GameSession::State::kHitStandPhase;
	
	if (m_dbContext.getGameSession().state == noGame)
	{
		m_sendHelper.whisperMsg(connection, m_views.noGames_View());
		return -1;
	}
	
	if (m_dbContext.getGameSession().state != hsPhase)
	{
		m_sendHelper.whisperMsg(connection, m_views.notHitStandPhase());
		return -1;
	}
	
	const auto& user = *m_dbContext.getUser(connection);
	if (!m_userManager.userIsAPlayer(user))
	{
		m_sendHelper.whisperMsg(connection, m_views.notAPlayer_View());
		return -1;
	}
	
	auto& player = *m_dbContext.getPlayers()[user.uniqueId];
	
	std::string exception("");
	try
	{
		m_playerLogic.executeStand(player, 0);
	}
	catch (PlayerLogic::NoSuchHand)
	{
		exception = m_views.noSuchHand_View(player.hands.size());
	}
	catch (PlayerLogic::HandIsNotSitting)
	{
		exception = m_views.handNotSitting_View(0);
	}
	
	if (!exception.empty())
	{
		m_logger.logAction(connection, "Hit: " + exception);
		m_sendHelper.whisperMsg(connection, exception);
		return -1;
	}
	
	const auto view = m_views.successfullStand({ &player });
	m_logger.logAction(connection, view);
	m_sendHelper.broadcastMsg(view);
	
	if (m_gmStatusLogic.allHandsAreStanding())
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
		
		m_dbContext.getPlayers().insert(
			std::make_pair(user->uniqueId, newPlayer));
	}
}
