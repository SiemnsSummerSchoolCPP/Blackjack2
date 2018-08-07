#pragma once

#include "Connection.h"
#include "Requests/Request.h"
#include <functional>
#include <vector>

namespace SocketConnection
{
	class Client
	{
	public:
		typedef std::function<void (const Connection&)> ConnectedEvent;
		typedef std::function<void (const Connection&)> DisconnectedEvent;
		typedef std::function<void (const Connection&, const Requests::Request&)> NewMsgEvent;
	
		// Getters.
		const Connection& getConnection() const;
		std::vector<ConnectedEvent>& getOnConnectEventHandlers();
		std::vector<DisconnectedEvent>& getOnDisconnectedEventHandlers();
		std::vector<NewMsgEvent>& getOnNewMsgEventHandlers();
		
		void setup(const char* ip, int port);
		int waitForAnActivity(time_t seconds = -1, suseconds_t micros = 0);
		void parseActivity();
		
	private:
		Connection m_connection;
		fd_set m_socketDescriptorsSet;
		
		std::vector<ConnectedEvent> m_onConnectEventHandlers;
		std::vector<DisconnectedEvent> m_onDisconnectedEventHandlers;
		std::vector<NewMsgEvent> m_onNewMsgEventHandlers;
	};
}
