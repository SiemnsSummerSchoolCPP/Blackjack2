#include "Controllers/LobbyController.hpp"
#include "Views/LobbyViews.hpp"
#include "SocketIO/Writer.hpp"
#include "BlackjackClient/Controllers/MsgReadController.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <regex>

using namespace BlackjackServer::Controllers;
using namespace BlackjackServer::Models;

static const auto kReceiveMsgHeader =
	BlackjackClient
		::Controllers
		::MsgReadController
		::RequestHeaders
		::kReceiveMsg;

LobbyController::LobbyController(
	const NetworkTools::NetworkHost& networkHost,
	Views::LobbyViews& views,
	const Services::Logger& logger,
	DataLayer::BjDatabase& dbContext) :
	m_networkHost(networkHost),
	m_views(views),
	m_logger(logger),
	m_dbContext(dbContext)
{
}

int LobbyController::clientJoin(
	const SocketConnection::Connection& connection,
	const Requests::Request& request) const
{
	auto user = new DataLayer::UserModel();
	user->name = generateGuestName(connection);
	user->uniqueId = connection.socket;
	m_dbContext.getUsers().push_back(user);

	auto model = Models::LobbyModels::ClientJoinModel();
	model.name = user->name;
	
	const auto view = m_views.clientJoin_View(model);
	auto resultRequest = Requests::Request(kReceiveMsgHeader, view);
	
	m_logger.logAction(connection, view);
	m_networkHost.broadcastRequest(resultRequest);
	return 0;
}

int LobbyController::clientLeave(
	const SocketConnection::Connection& connection,
	const Requests::Request& request) const
{
	const auto user = m_dbContext.getUser(connection);

	auto model = Models::LobbyModels::ClientLeaveModel();
	model.name = user->name;
	
	const auto view = m_views.clientClientLeave_View(model);
	auto resultRequest = Requests::Request(kReceiveMsgHeader, view);
	
	m_logger.logAction(connection, view);
	m_networkHost.broadcastRequest(resultRequest);
	return 0;
}

int LobbyController::sendMsg(
	const SocketConnection::Connection& connection,
	const Requests::Request& request) const
{
	const auto user = m_dbContext.getUser(connection);

	auto model = Models::LobbyModels::SendMsgModel();
	model.name = user->name;
	model.msg = reinterpret_cast<const char*>(request.data);
	
	const auto view = m_views.sendMsg_View(model);
	auto resultRequest = Requests::Request(kReceiveMsgHeader, view);
	m_logger.logAction(connection, view);
	m_networkHost.broadcastRequest(resultRequest);
	return 0;
}

int LobbyController::changeName(
	const SocketConnection::Connection& connection,
	const Requests::Request& request) const
{
	auto user = m_dbContext.getUser(connection);
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
	
	const auto view = m_views.changeName_View(model);
	auto resultRequest = Requests::Request(kReceiveMsgHeader, view);
	m_logger.logAction(connection, view);
	m_networkHost.broadcastRequest(resultRequest);
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
	auto user = m_dbContext.getUser(connection);
	
	const auto whisper = m_views.invalidName_WhisperView(
		invalidName,
		invalidNameMsg);

	const auto resultRequest = Requests::Request(
		kReceiveMsgHeader,
		whisper);

	SocketIO::Writer::getWriter().write(connection.socket, resultRequest);

	auto serverModel = LobbyModels::InvalidChangeNameModel_Server();
	serverModel.oldName = user->name;
	serverModel.newName = invalidName;
	serverModel.errorMsg = invalidNameMsg;

	const auto view = m_views.invalidName_ServerView(serverModel);
	m_logger.logAction(connection, view);
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
	
	const auto& allUsers = m_dbContext.getUsers();
	const auto findResult = std::find_if(
		allUsers.begin(),
		allUsers.end(),
		[&](const DataLayer::UserModel* user)
		{
			return user->name == name;
		});
	
	if (findResult != allUsers.end())
		return "Name already taken.";
	
	return nullptr;
}
