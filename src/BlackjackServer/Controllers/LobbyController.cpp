#include "Controllers/LobbyController.hpp"
#include "BlackjackClient/Controllers/MsgReadController.hpp"
#include <iostream>

using namespace BlackjackServer;
using namespace BlackjackClient::Controllers;

LobbyController::LobbyController(const NetworkTools::NetworkHost& networkHost) :
	m_networkHost(networkHost)
{
}

int LobbyController::sendMsg(
	const SocketConnection::Connection& connection,
	const Requests::Request& request) const
{
	Requests::Request resultRequest(request);
	
	resultRequest.header =
		MsgReadController::RequestHeaders::kReceiveMsg;
	m_networkHost.broadcastRequest(resultRequest);
	return 0;
}
