#pragma once

#include "BlackjackServer/Views/GameSessionViews.hpp"
#include "Services/Logger.hpp"
#include "Services/SendHelper.hpp"
#include "Services/UserManager.hpp"
#include "BlackjackLogic/GameStatusLogic.hpp"
#include "BlackjackLogic/PlayerLogic.hpp"
#include "BlackjackLogic/DealerLogic.hpp"
#include "DataLayer/UserModel.h"

namespace BlackjackServer { namespace Controllers
{
	class GameSessionController
	{
	public:
		enum RequestHeaders : Requests::RequestHeader
		{
			kPlaceBet	= 200 + 0,
			kHit		= 200 + 1,
			kStand		= 200 + 2
		};
		
		GameSessionController(
			const Views::GameSessionViews* views,
			const Services::Logger* logger,
			const Services::SendHelper* sendHelper,
			const Services::UserManager* userManager,
			DataLayer::BjDatabase* dbContext,
			
			const BlackjackLogic::GameStatusLogic* gmStatusLogic,
			const BlackjackLogic::PlayerLogic* playerLogic,
			const BlackjackLogic::DealerLogic* dealerLogic
		);
	
		void startGame(const std::vector<DataLayer::UserModel*>& users) const;
		void startHitStandPhase() const;
		void startCashing() const;
		
		void endGame() const;
		void leaveGame(
			const SocketConnection::Connection& connection,
			const DataLayer::PlayerModel& player) const;
		
		int betRequest(
			const SocketConnection::Connection& connection,
			const Requests::Request& request) const;
		int hitRequest(
			const SocketConnection::Connection& connection,
			const Requests::Request& request) const;
		int standRequest(
			const SocketConnection::Connection& connection,
			const Requests::Request& request) const;
	
	private:
		const Views::GameSessionViews* m_views;
		const Services::Logger* m_logger;
		const Services::SendHelper* m_sendHelper;
		const Services::UserManager* m_userManager;
		DataLayer::BjDatabase* m_dbContext;
		
		const BlackjackLogic::GameStatusLogic* m_gmStatusLogic;
		const BlackjackLogic::PlayerLogic* m_playerLogic;
		const BlackjackLogic::DealerLogic* m_dealerLogic;
		
		void createThePlayers(
			const std::vector<DataLayer::UserModel*>& users) const;
		void printInitialCardsSetup() const;
		void cashPlayers() const;
		
		bool basicRequestValidation(
			DataLayer::GameSession::State requiredGameState,
			const SocketConnection::Connection& connection) const;
		
		bool tryExecuteBet(
			const SocketConnection::Connection& connection,
			DataLayer::PlayerModel& player,
			const DataLayer::Bet& bet) const;
		
		const PlayingCards::Card* tryExecuteHit(
			const SocketConnection::Connection& connection,
			DataLayer::PlayerModel& player) const;
	};
}}
