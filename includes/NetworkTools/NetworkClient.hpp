#pragma once

#include "NetworkClientModel.h"
#include "NetworkEntity.hpp"
#include "SocketConnection/Client.hpp"

namespace NetworkTools
{
	class NetworkClient : public NetworkEntity
	{
	public:
		NetworkClient(const NetworkClientModel& model);
		int sendRequest(const Requests::Request& request);
		void parseAnyIncomingActivities();
		
	private:
		SocketConnection::Client m_client;
	};
}
