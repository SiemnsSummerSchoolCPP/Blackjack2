#include "Controllers/GameSessionController.hpp"
#include <sstream>

using namespace BlackjackLogic;
using namespace BlackjackServer::Controllers;
using namespace BlackjackServer::Models;

GameSessionController::GameSessionController(
	const Views::GameSessionViews& views,
	const Services::Logger& logger,
	const Services::SendHelper& sendHelper,
	const Services::UserManager& userManager,
	DataLayer::BjDatabase& dbContext,
	
	BlackjackLogic::GameStatusLogic& gmStatusLogic,
	BlackjackLogic::PlayerLogic& playerLogic
) :
	m_views(views),
	m_logger(logger),
	m_sendHelper(sendHelper),
	m_userManager(userManager),
	m_dbContext(dbContext),
	m_gmStatusLogic(gmStatusLogic),
	m_playerLogic(playerLogic)
{
}

void GameSessionController::startGame(
	const std::vector<DataLayer::UserModel*>& users)
{
	const auto view = m_views.startGame_View({ &users });
	m_sendHelper.broadcastMsg(view);
	m_logger.logAction(view);
	
	createThePlayers(users);
	m_gmStatusLogic.setBettingPhase();
	
	const auto betPhaseView = m_views.startBettingPhase_View();
	m_sendHelper.broadcastMsg(betPhaseView);
	m_logger.logAction(betPhaseView);
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
	
	auto& player = m_dbContext.getPlayers()[user.uniqueId];
	double betAmount = 0;
	int handIndex = 0;
	
	memcpy(&betAmount, request.data, sizeof(betAmount));

	return 0;
}

/*
** Private methods.
*/

void GameSessionController::createThePlayers(
	const std::vector<DataLayer::UserModel*>& users)
{
	for (const auto& user : users)
	{
		auto newPlayer = new DataLayer::PlayerModel({ user });
		m_dbContext.getPlayers().insert(
			std::make_pair(user->uniqueId, newPlayer));
	}
}
