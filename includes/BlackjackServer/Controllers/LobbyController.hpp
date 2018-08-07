#pragma once

#include "BlackjackServer/Views/LobbyViews.hpp"
#include "SocketConnection/Connection.h"
#include "Requests/Request.h"
#include "NetworkTools/NetworkHost.hpp"
#include "Services/Logger.hpp"
#include "DataLayer/BjDatabase.hpp"

namespace BlackjackServer { namespace Controllers
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
		
		LobbyController(
			const NetworkTools::NetworkHost& networkHost,
			Views::LobbyViews& views,
			const Services::Logger& logger,
			DataLayer::BjDatabase& dbContext);
		
		int clientJoin(
			const SocketConnection::Connection& connection,
			const Requests::Request& request) const;
		
		int clientLeave(
			const SocketConnection::Connection& connection,
			const Requests::Request& request) const;
		
		int sendMsg(
			const SocketConnection::Connection& connection,
			const Requests::Request& request) const;
		
		int changeName(
			const SocketConnection::Connection& connection,
			const Requests::Request& request) const;
		
	private:
		static const int maxNameLength = 13;
		static const int minNameLength = 3;
	
		const NetworkTools::NetworkHost& m_networkHost;
		Views::LobbyViews& m_views;
		const Services::Logger& m_logger;
		DataLayer::BjDatabase& m_dbContext;
		
		void handleInvalidName(
			const SocketConnection::Connection& connection,
			const std::string& invalidName,
			const char* invalidNameMsg) const;
		
		std::string generateGuestName(
			const SocketConnection::Connection& connection) const;
		const char* nameIsValid(std::string name) const;
	};
}}
