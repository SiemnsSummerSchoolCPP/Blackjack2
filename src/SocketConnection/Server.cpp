#include "Server.hpp"
#include "Exceptions.h"
#include "SocketIO/Reader.hpp"
#include <memory>
#include <cerrno>
#include <sys/select.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>

#define STR(someStr) (std::string() + someStr)

using namespace SocketConnection;
using namespace SocketConnection::Exceptions;

Server::Server(
	const size_t maxNbOfClients,
	const size_t maxPendingConnections) :
	m_maxNbOfClients(maxNbOfClients),
	m_maxPendingConnections(maxPendingConnections)
{
}

/*
** Getters
*/

const std::vector<Connection>& Server::getConnections() const
{
	return m_connections;
}

size_t Server::getMaxNbOfClients() const
{
	return m_maxNbOfClients;
}

std::vector<Server::ClientConnectedEvent>& Server::getNewClientEventHandlers()
{
	return m_newClientEventHandlers;
}

std::vector<Server::ClientDisconnectedEvent>&
Server::getDisconnectedEventHandlers()
{
	return m_clientDisconnectedEventHandlers;
}

std::vector<Server::ClientMsgEvent>& Server::getClientMsgEventHandlers()
{
	return m_newMsgEventHandlers;
}

/*
** Setup.
*/

static int createMasterSocket()
{
	const auto result = socket(AF_INET, SOCK_STREAM, 0);
	if (result == -1)
		throw SocketExceptionStr(STR("socket: " + std::strerror(errno)));
	
	return result;
}

static void allowMultipleConnections(const int socket)
{
	int options = true;
	
	const auto setsockoptResult = setsockopt(
		socket,
		SOL_SOCKET,
		SO_REUSEADDR,
		(char*)&options,
		sizeof(options));
	
	if (setsockoptResult == -1)
		throw SocketExceptionStr(STR("setsockopt: " + std::strerror(errno)));
}

static void bindSocketToLocalHost(
	const int socket,
	const sockaddr* const address)
{
	if (bind(socket, address, sizeof(*address)) == -1)
		throw SocketExceptionStr(STR("bind: " + std::strerror(errno)));
}

void Server::setup(const int port)
{
	m_masterSocket = createMasterSocket();
	allowMultipleConnections(m_masterSocket);
	
	// Set type of socket created.
    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_port = htons(port);
	
    bindSocketToLocalHost(
		m_masterSocket,
		reinterpret_cast<sockaddr*>(&m_address));
	
	if (listen(m_masterSocket, m_maxPendingConnections) == -1)
		throw SocketExceptionStr(STR("listen: " + std::strerror(errno)));
}

static int addSocketsToReadSet(
	const std::vector<Connection>& connections,
	fd_set& socketDescriptorSet)
{
	auto maxSocketDescriptor = -1;
	
	for (const auto& connection : connections)
	{
		FD_SET(connection.socket, &socketDescriptorSet);
		if (connection.socket > maxSocketDescriptor)
		{
			maxSocketDescriptor = connection.socket;
		}
	}

	return maxSocketDescriptor;
}

int Server::waitForAnActivity(
	const time_t seconds,
	const suseconds_t microseconds)
{
	FD_ZERO(&m_socketDescriptorsSet);
	
	// Add master socket to set.
	FD_SET(m_masterSocket, &m_socketDescriptorsSet);
	auto clientsMaxSockDescript = addSocketsToReadSet(
		m_connections,
		m_socketDescriptorsSet);

	const auto maxSocketDescriptor = std::max(
		m_masterSocket,
		clientsMaxSockDescript);

	timeval timeout;
	memset(&timeout, 0, sizeof(timeout));
	timeout.tv_sec = seconds;
	timeout.tv_usec = microseconds;

	const auto nbOfReadySockets = select(
		maxSocketDescriptor + 1,
		&m_socketDescriptorsSet,
		nullptr,
		nullptr,
		(seconds < 0) ? nullptr : &timeout);

	if (nbOfReadySockets == -1)
		throw SocketExceptionStr(STR("select: " + std::strerror(errno)));

	return nbOfReadySockets;
}

void Server::parseActivity()
{
	if (FD_ISSET(m_masterSocket, &m_socketDescriptorsSet))
		parseServerActivity();
	
	for (const auto& connection : m_connections)
	{
		if (FD_ISSET(connection.socket, &m_socketDescriptorsSet))
			parseClientActivity(connection);
	}
}

/*
** State getters.
*/

size_t Server::getNbOfConnections() const
{
	return m_connections.size();
}

/*
** Helpers
*/

void Server::parseServerActivity()
{
	socklen_t addrLen = sizeof(m_address);
	const auto newSocket = accept(
		m_masterSocket,
		reinterpret_cast<sockaddr*>(&m_address),
		&addrLen);

	if (newSocket == -1)
		throw SocketExceptionStr(STR("accept: " + std::strerror(errno)));

	auto connection = Connection();
	connection.socket = newSocket;
	connection.address = m_address;
	m_connections.push_back(connection);

	for (const auto& eventHandler : m_newClientEventHandlers)
		eventHandler(connection);
}

void Server::parseClientActivity(const Connection& connection)
{
	Requests::Request request;
	
	const auto readResult = SocketIO::Reader::getInstance().read(
		connection.socket,
		request);
	
	if (readResult > 0)
	{
		for (const auto& eventHandlerF : m_newMsgEventHandlers)
		{
			eventHandlerF(connection, request);
		}
	}
	else if (readResult == 0)
	{
		if (close(connection.socket) == -1)
			throw SocketExceptionStr(STR("close: " + std::strerror(errno)));

		for (const auto& eventHandlerF : m_clientDisconnectedEventHandlers)
			eventHandlerF(connection);
		
		m_connections.erase(
			std::find_if(
				m_connections.begin(),
				m_connections.end(),
				[&](const Connection& iter) { return &iter == &connection; }));
	}
	else
	{
		throw SocketExceptionStr(STR("read: " + std::strerror(errno)));
	}
}
