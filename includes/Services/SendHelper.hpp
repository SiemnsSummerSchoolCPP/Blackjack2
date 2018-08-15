#pragma once

#include "Requests/Request.h"
#include "NetworkTools/NetworkHost.hpp"
#include "Services/Logger.hpp"
#include <string>

namespace Services
{
	class SendHelper
	{
	public:
		SendHelper(
			const NetworkTools::NetworkHost& networkHost,
			Services::Logger& logger,
			Requests::RequestHeader msgHeader);
	
		void whisperMsg(int socket, const std::string& msg) const;
		void whisperMsg(
			const SocketConnection::Connection& connection,
			const std::string& msg) const;
		
		void broadcastMsg(
			const std::string& msg,
			const SocketConnection::Connection* exceptconnection = nullptr
		) const;
		
		void logAndBroadcast(const std::string& msg) const;
		
	private:
		const NetworkTools::NetworkHost& m_networkHost;
		Services::Logger& m_logger;
		const Requests::RequestHeader m_msgHeader;
		
		std::string appendCandyStuff(const std::string& msg) const;
		std::string makeMsgPrefix() const;
	};
}
