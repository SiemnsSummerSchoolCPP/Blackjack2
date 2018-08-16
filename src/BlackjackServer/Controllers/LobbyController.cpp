#include "Controllers/LobbyController.hpp"
#include "SocketIO/Writer.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <regex>

using namespace BlackjackServer::Controllers;
using namespace BlackjackServer::Models;
using namespace DataLayer;

LobbyController::LobbyController(
	const Views::LobbyViews* views,
	const Services::Logger* logger,
	const Services::SendHelper* sendHelper,
	const Services::UserManager* userManager,
	DataLayer::BjDatabase* dbContext,
	const GameSessionController* gmSessionCtrl) :

	m_views(views),
	m_logger(logger),
	m_sendHelper(sendHelper),
	m_userManager(userManager),
	m_dbContext(dbContext),
	m_gmSessionCtrl(gmSessionCtrl)
{
}

int LobbyController::clientJoin(
	const SocketConnection::Connection& connection,
	const Requests::Request& request) const
{
	auto user = new DataLayer::UserModel();
	user->name = generateGuestName(connection);
	user->uniqueId = connection.socket;
	user->money = LobbyController::initialMoney;
	m_dbContext->users.insert(
		std::make_pair(connection.socket, UserModelPtr(user)));

	auto model = Models::LobbyModels::ClientJoinModel();
	model.name = user->name;
	
	const auto view = m_views->clientJoin_View(model);
	m_logger->logAction(connection, view);
	m_sendHelper->broadcastMsg(view);
	return 0;
}

int LobbyController::clientLeave(
	const SocketConnection::Connection& connection,
	const Requests::Request& request) const
{
	const auto& user = *m_dbContext->getUser(connection);

	auto model = Models::LobbyModels::ClientLeaveModel();
	model.name = user.name;

	const auto view = m_views->clientClientLeave_View(model);
	m_logger->logAction(connection, view);
	m_sendHelper->broadcastMsg(view);
	
	if (m_userManager->userIsAPlayer(user))
	{
		if (m_dbContext->players.size() == 1)
		{
			const auto view = m_views->allPlayersLeft_View();
			m_logger->logAction(view);
			m_sendHelper->broadcastMsg(view);
		}
	
		m_gmSessionCtrl->leaveGame(
			connection,
			*m_dbContext->players[user.uniqueId]);
	}
	
	m_dbContext->users.erase(user.uniqueId);
	return 0;
}

int LobbyController::sendMsg(
	const SocketConnection::Connection& connection,
	const Requests::Request& request) const
{
	const auto user = m_dbContext->getUser(connection);

	auto model = Models::LobbyModels::SendMsgModel();
	model.name = user->name;
	model.msg = reinterpret_cast<const char*>(request.data);

	const auto view = m_views->sendMsg_View(model);
	m_logger->logAction(connection, view);
	m_sendHelper->broadcastMsg(view);
	return 0;
}

int LobbyController::changeName(
	const SocketConnection::Connection& connection,
	const Requests::Request& request) const
{
	auto user = m_dbContext->getUser(connection);
	auto model = Models::LobbyModels::ChangeNameModel();
	model.oldName = user->name;
	model.newName = std::string(reinterpret_cast<const char*>(request.data));

	const auto& nameException = nameIsValid(model.newName.c_str());
	if (nameException)
	{
		handleInvalidName(connection, model.newName, nameException);
		return -1;
	}

	user->name = model.newName;

	const auto view = m_views->changeName_View(model);
	m_logger->logAction(connection, view);
	m_sendHelper->broadcastMsg(view);
	return 0;

}

int LobbyController::setReady(
	const SocketConnection::Connection& connection,
	const Requests::Request& request) const
{
	auto user = m_dbContext->getUser(connection);
	if (user->joinState.isReady)
	{
		handleAlreadyReady(connection);
		return -1;
	}
	
	if (m_dbContext->gameSession.state != GameSession::State::kNotStarted)
	{
		m_logger->logAction(
			connection,
			m_views->setReadyAnotherGameIsBeingPlayed_ServerView());
		m_sendHelper->whisperMsg(
			connection,
			m_views->anotherGameIsBeingPlayed_View());
		return -1;
	}
	
	if (!m_userManager->userCanJoin(*user))
	{
		m_sendHelper->whisperMsg(
			connection,
			m_views->userIsUnableToPlay_View(*user));
		return -1;
	}
	
	user->joinState.isReady = true;
	const auto view = m_views->successSetReady_View(user->name);
	m_logger->logAction(connection, view);
	m_sendHelper->broadcastMsg(view);
	
	if (m_userManager->gameIsReady())
	{
		m_gmSessionCtrl->startGame(m_userManager->getJoinables());
	}
	else
	{
		const auto namesToBeWaited = getNamesToBeWaited();
		if (namesToBeWaited.size() == 0)
			return 0;
		
		m_sendHelper->broadcastMsg(
			m_views->waitingForOthers_View(namesToBeWaited));
	}
	return 0;
}

/*
** Private methods.
*/

void LobbyController::handleInvalidName(
	const SocketConnection::Connection& connection,
	const std::string& invalidName,
	const char* invalidNameMsg) const
{
	auto user = m_dbContext->getUser(connection);

	m_sendHelper->whisperMsg(connection,
		m_views->invalidName_WhisperView(
			invalidName,
			invalidNameMsg));

	auto serverModel = LobbyModels::InvalidChangeNameModel_Server();
	serverModel.oldName = user->name;
	serverModel.newName = invalidName;
	serverModel.errorMsg = invalidNameMsg;
	m_logger->logAction(
		connection,
		m_views->invalidName_ServerView(serverModel));
}

void LobbyController::handleAlreadyReady(
	const SocketConnection::Connection& connection) const
{
	auto user = m_dbContext->getUser(connection);

	m_logger->logAction(
		connection,
		m_views->alreadyReady_ServerView(user->name));
	m_sendHelper->whisperMsg(connection, m_views->alreadyReady_WhisperView());
	
	auto namesToBeWaited = getNamesToBeWaited();
	if (namesToBeWaited.size() == 0)
		return;
	
	m_sendHelper->whisperMsg(
		connection,
		m_views->waitingForOthers_View(namesToBeWaited));
}

/*
** Helpers
*/

std::vector<std::string> LobbyController::getNamesToBeWaited() const
{
	const auto usersToWait = m_userManager->getUsersToWait();
	std::vector<std::string> namesToBeWaited;
	
	namesToBeWaited.resize(usersToWait.size());
	std::transform(
		usersToWait.begin(),
		usersToWait.end(),
		namesToBeWaited.begin(),
		[](const auto& user)
		{
			return user->name;
		});
	return namesToBeWaited;
}

std::string LobbyController::generateGuestName(
	const SocketConnection::Connection& connection) const
{
	return
		std::string("Guest") +
		std::to_string(ntohs(connection.address.sin_port));
}

const char* LobbyController::nameIsValid(std::string name) const
{
	const auto pattern = "^[a-z0-9]*$";
	if (!std::regex_match(name, std::regex(pattern, std::regex::icase)))
		return "Invalid characters.";
	
	if (name.length() > maxNameLength)
		return "Name too long.";
	if (name.length() < minNameLength)
		return "Name too short.";
	
	const auto& allUsers = m_dbContext->users;
	const auto findResult = std::find_if(
		allUsers.begin(),
		allUsers.end(),
		[&](const auto& pair)
		{
			return pair.second->name == name;
		});
	
	if (findResult != allUsers.end())
		return "Name already taken.";
	
	return nullptr;
}
