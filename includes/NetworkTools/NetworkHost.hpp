#pragma once

#include "NetworkHostModel.h"
#include "RequestMapper.hpp"
#include "NetworkEntity.hpp"
#include "SocketConnection/Server.hpp"
#include "SocketConnection/Connection.h"
#include "Requests/Request.h"

namespace NetworkTools
{
	class NetworkHost : public NetworkEntity
	{
	public:
		NetworkHost(const NetworkHostModel& model);
		
		void parseAnyIncomingActivities();
		
		int broadcastRequest(
			const Requests::Request& request,
			const SocketConnection::Connection* exceptConnection = nullptr
		) const;
		
	private:
		SocketConnection::Server m_server;
	};
}
