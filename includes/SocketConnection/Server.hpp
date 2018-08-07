#pragma once

#include "Connection.h"
#include "Requests/Request.h"
#include <string>
#include <vector>
#include <functional>

namespace SocketConnection
{
	class Server
	{
	public:
		// Event handlers' types.
		typedef std::function<void (const Connection&)> ClientConnectedEvent;
		typedef std::function<void (const Connection&)> ClientDisconnectedEvent;
		typedef std::function<void (const Connection&, const Requests::Request&)> ClientMsgEvent;
	
		// Constructors.
		Server(size_t maxNbOfClients = 4, size_t maxPendingConnections = 5);
		
		// Getters.
		const std::vector<Connection>& getConnections() const;
		size_t getMaxNbOfClients() const;
		
		std::vector<ClientConnectedEvent>& getNewClientEventHandlers();
		std::vector<ClientDisconnectedEvent>& getDisconnectedEventHandlers();
		std::vector<ClientMsgEvent>& getClientMsgEventHandlers();
		
		// Public methods.
		void setup(int port);
		int waitForAnActivity(time_t seconds = -1, suseconds_t micros = 0);
		void parseActivity();
		
		// Status get.
		size_t getNbOfConnections() const;
		
	private:
		const size_t m_maxNbOfClients;
		const size_t m_maxPendingConnections;
	
		int m_masterSocket = 0;
		std::vector<Connection> m_connections;
		fd_set m_socketDescriptorsSet;
		sockaddr_in m_address;
		
		std::vector<ClientConnectedEvent> m_newClientEventHandlers;
		std::vector<ClientDisconnectedEvent> m_clientDisconnectedEventHandlers;
		std::vector<ClientMsgEvent> m_newMsgEventHandlers;
		
		// Private helpers.
		void parseServerActivity();
		void parseClientActivity(const Connection& connection);
	};
}
