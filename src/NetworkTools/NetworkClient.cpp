#include "NetworkClient.hpp"
#include "SocketIO/Writer.hpp"

using namespace NetworkTools;

NetworkClient::NetworkClient(const NetworkClientModel& model) :
	NetworkEntity(*model.requestMapper),
	m_client()
{
	m_client.setup(model.ip, model.port);
	
	m_client.getOnConnectEventHandlers().push_back(
		[&](const SocketConnection::Connection& connection)
		{
			this->onNewConnection(connection);
		});
	
	m_client.getOnDisconnectedEventHandlers().push_back(
		[&](const SocketConnection::Connection& connection)
		{
			this->onEndConnection(connection);
		});
	
	m_client.getOnNewMsgEventHandlers().push_back(
		[&](const SocketConnection::Connection& connection,
			const Requests::Request& request)
		{
			this->onConnectionMsg(connection, request);
		});
}

int NetworkClient::sendRequest(const Requests::Request& request) const
{
	return SocketIO::Writer::getWriter().write(
		m_client.getConnection().socket,
		request);
}

void NetworkClient::parseAnyIncomingActivities()
{
	m_client.waitForAnActivity();
	m_client.parseActivity();
}
