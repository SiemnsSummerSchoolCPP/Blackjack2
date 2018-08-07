#include "Client.hpp"
#include "Exceptions.h"
#include "SocketIO/Reader.hpp"
#include <sys/socket.h>
#include <cerrno>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define STR(someStr) (std::string() + someStr)

using namespace SocketConnection;
using namespace SocketConnection::Exceptions;

const Connection& Client::getConnection() const
{
	return m_connection;
}

std::vector<Client::ConnectedEvent>& Client::getOnConnectEventHandlers()
{
	return m_onConnectEventHandlers;
}

std::vector<Client::DisconnectedEvent>& Client::getOnDisconnectedEventHandlers()
{
	return m_onDisconnectedEventHandlers;
}

std::vector<Client::NewMsgEvent>& Client::getOnNewMsgEventHandlers()
{
	return m_onNewMsgEventHandlers;
}

void Client::setup(const char* ip, int port)
{
	int socketDescriptor = -1;
	
	if ((socketDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw SocketExceptionStr(STR("socket: " + std::strerror(errno)));
	
	sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	
	if (inet_pton(AF_INET, ip, &serverAddr.sin_addr) == -1)
		throw SocketExceptionStr(STR("inet_pton: " + std::strerror(errno)));
	
	const auto connectResult = connect(
		socketDescriptor,
		reinterpret_cast<sockaddr*>(&serverAddr),
		sizeof(serverAddr));
	
	if (connectResult == -1)
		throw SocketExceptionStr(STR("connect: " + std::strerror(errno)));
	
	m_connection.socket = socketDescriptor;
	m_connection.address = *reinterpret_cast<sockaddr*>(&serverAddr);
	
	for (const auto& eventHandler : m_onConnectEventHandlers)
	{
		eventHandler(m_connection);
	}
}

int Client::waitForAnActivity(time_t seconds, suseconds_t micros)
{
	FD_ZERO(&m_socketDescriptorsSet);
	FD_SET(m_connection.socket, &m_socketDescriptorsSet);

	timeval timeout;
	memset(&timeout, 0, sizeof(timeout));
	timeout.tv_sec = seconds;
	timeout.tv_usec = micros;

	const auto nbOfReadySockets = select(
		m_connection.socket + 1,
		&m_socketDescriptorsSet,
		nullptr,
		nullptr,
		(seconds < 0) ? nullptr : &timeout);

	if (nbOfReadySockets == -1)
	{
		throw SocketExceptionStr(STR("select: " + std::strerror(errno)));
	}

	return nbOfReadySockets;
}

void Client::parseActivity()
{
	if (!FD_ISSET(m_connection.socket, &m_socketDescriptorsSet))
		return;

	Requests::Request request;
	const auto readResult = SocketIO::Reader::getInstance().read(
		m_connection.socket,
		request);

	if (readResult > 0)
	{
		for (const auto& eventHandlerF : m_onNewMsgEventHandlers)
		{
			eventHandlerF(m_connection, request);
		}
	}
	else if (readResult == 0)
	{
		if (close(m_connection.socket) == -1)
			throw SocketExceptionStr(STR("close: " + std::strerror(errno)));

		for (const auto& eventHandlerF : m_onDisconnectedEventHandlers)
			eventHandlerF(m_connection);
	}
	else
	{
		throw SocketExceptionStr(STR("read: " + std::strerror(errno)));
	}
}
