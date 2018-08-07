#pragma once

#include "SocketConnection/Connection.h"
#include "Requests/Request.h"
#include "NetworkTools/NetworkHost.hpp"

namespace BlackjackServer
{
	class LobbyController
	{
	public:
		enum RequestHeaders : Requests::RequestHeader
		{
			kSendMsg =	100 + 0,
			kSetName =	100 + 1,
			kSetReady = 100 + 2
		};
		
		LobbyController(const NetworkTools::NetworkHost& networkHost);
		
		int sendMsg(
			const SocketConnection::Connection& connection,
			const Requests::Request& request) const;
		
	private:
		const NetworkTools::NetworkHost& m_networkHost;
	};
}
