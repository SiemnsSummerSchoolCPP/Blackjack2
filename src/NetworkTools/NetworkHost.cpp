#include "NetworkHost.hpp"
#include "SocketIO/Writer.hpp"

using namespace NetworkTools;

NetworkHost::NetworkHost(const NetworkHostModel& model) :
	NetworkEntity(*model.requestMapper),
	m_server(model.maxConnections, model.maxPendings)
{
	m_server.setup(model.port);
	
	m_server.getNewClientEventHandlers().push_back(
		[&](const SocketConnection::Connection& connection)
		{
			this->onNewConnection(connection);
		});
	
	m_server.getDisconnectedEventHandlers().push_back(
		[&](const SocketConnection::Connection& connection)
		{
			this->onEndConnection(connection);
		});
	
	m_server.getClientMsgEventHandlers().push_back(
		[&](
			const SocketConnection::Connection& connection,
			const Requests::Request& request)
		{
			this->onConnectionMsg(connection, request);
		});
}

void NetworkHost::parseAnyIncomingActivities()
{
	m_server.waitForAnActivity();
	m_server.parseActivity();
}

int NetworkHost::broadcastRequest(
	const Requests::Request& request,
	const SocketConnection::Connection* exceptConnection) const
{
	std::vector<int> sockets;	
	for (const auto& connection : m_server.getConnections())
	{
		if (exceptConnection != nullptr &&
			exceptConnection->socket != connection.socket)
		{
			continue;
		}
		
		sockets.push_back(connection.socket);
	}
	
	return SocketIO::Writer::getWriter().broadcast(sockets, request);
}
