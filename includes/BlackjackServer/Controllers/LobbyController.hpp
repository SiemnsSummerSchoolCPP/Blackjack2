#pragma once

#include "BlackjackServer/Views/LobbyViews.hpp"
#include "SocketConnection/Connection.h"
#include "Requests/Request.h"
#include "NetworkTools/NetworkHost.hpp"
#include "Services/Logger.hpp"
#include "Services/UserManager.hpp"
#include "Services/SendHelper.hpp"
#include "DataLayer/BjDatabase.hpp"
#include "BlackjackServer/Controllers/GameSessionController.hpp"

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
		
		constexpr static const double initialMoney = 1000;
		
		LobbyController(
			Views::LobbyViews& views,
			const Services::Logger& logger,
			const Services::SendHelper& sendHelper,
			Services::UserManager& userManager,
			DataLayer::BjDatabase& dbContext,
			GameSessionController* gmSessionCtrl);
		
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
		
		int setReady(
			const SocketConnection::Connection& connection,
			const Requests::Request& request) const;
		
		// Other.
		void startGame() const;
		
	private:
		static const int maxNameLength = 13;
		static const int minNameLength = 3;
	
		Views::LobbyViews& m_views;
		const Services::Logger& m_logger;
		const Services::SendHelper& m_sendHelper;
		Services::UserManager& m_userManager;
		DataLayer::BjDatabase& m_dbContext;
		GameSessionController* m_gmSessionCtrl;
		
		void handleInvalidName(
			const SocketConnection::Connection& connection,
			const std::string& invalidName,
			const char* invalidNameMsg) const;
		
		void handleAlreadyReady(
			const SocketConnection::Connection& connection) const;
		
		std::vector<std::string> getNamesToBeWaited() const;
		
		std::string generateGuestName(
			const SocketConnection::Connection& connection) const;
		const char* nameIsValid(std::string name) const;
	};
}}
