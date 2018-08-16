#include "AppControllers.h"
#include "AppViews.h"
#include "AppServices.h"
#include "AppLogic.h"

#include "NetworkTools/RequestMapper.hpp"
#include "NetworkTools/NetworkHost.hpp"
#include "NetworkTools/NetworkHostModel.h"
#include "SocketConnection/Exceptions.h"
#include "DataLayer/BjDatabase.hpp"

#include "Services/Logger.hpp"
#include "Services/SendHelper.hpp"
#include "Services/PrintHelper.hpp"

#include "BlackjackClient/Controllers/MsgReadController.hpp"
#include "Controllers/LobbyController.hpp"
#include "Controllers/GameSessionController.hpp"

#include <iostream>

using namespace BlackjackServer;
using namespace Requests;

#define ADD_CONTROLLER_ACTION(mapper, header, function)						\
	mapper.setAction(														\
		static_cast<RequestHeader>(header),									\
		[&](																\
			const SocketConnection::Connection& connection,					\
			const Request* request)											\
		{																	\
			return function(connection, *request);							\
		});																	\

static const RequestHeader kReceiveMsgHeader =
	BlackjackClient
		::Controllers
		::MsgReadController
		::RequestHeaders
		::kReceiveMsg;

static void addLobbyControllerActions(
	Controllers::LobbyController& ctrl,
	NetworkTools::RequestMapper& mapper)
{
	ADD_CONTROLLER_ACTION(
		mapper,
		NetworkTools::NetworkEntity::kNewConnectionHeader,
		ctrl.clientJoin);
	ADD_CONTROLLER_ACTION(
		mapper,
		NetworkTools::NetworkEntity::kEndConnectionHeader,
		ctrl.clientLeave);

	ADD_CONTROLLER_ACTION(
		mapper,
		Controllers::LobbyController::RequestHeaders::kSendMsg,
		ctrl.sendMsg);
	ADD_CONTROLLER_ACTION(
		mapper,
		Controllers::LobbyController::RequestHeaders::kSetName,
		ctrl.changeName);
	ADD_CONTROLLER_ACTION(
		mapper,
		Controllers::LobbyController::RequestHeaders::kSetReady,
		ctrl.setReady);
}

static void addGmSessionControllerActions(
	Controllers::GameSessionController& ctrl,
	NetworkTools::RequestMapper& mapper)
{
	ADD_CONTROLLER_ACTION(
		mapper,
		Controllers::GameSessionController::RequestHeaders::kPlaceBet,
		ctrl.betRequest);
	ADD_CONTROLLER_ACTION(
		mapper,
		Controllers::GameSessionController::RequestHeaders::kHit,
		ctrl.hitRequest);
	ADD_CONTROLLER_ACTION(
		mapper,
		Controllers::GameSessionController::RequestHeaders::kStand,
		ctrl.standRequest);
}

static NetworkTools::NetworkHost buildNetworkHost(
	const int port,
	const NetworkTools::RequestMapper& requestMapper)
{
	auto networkHostModel = NetworkTools::NetworkHostModel();
	networkHostModel.port = 8000;
	networkHostModel.requestMapper = &requestMapper;
	
	try
	{
		return NetworkTools::NetworkHost(networkHostModel);
	}
	catch (const SocketConnection::Exceptions::SocketException& e)
	{
		std::cerr << "Setup: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

void continouslyParseNetworkInput(NetworkTools::NetworkHost& networkHost)
{
	while (true)
	{
		try
		{
			networkHost.parseAnyIncomingActivities();
		}
		catch (SocketConnection::Exceptions::SocketException& e)
		{
			std::cerr << e.what() << std::endl;
		}
		catch (const NetworkTools::RequestMapper::NoSuchHeaderException& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

int main(const int argc, const char* const* const argv)
{
	if (argc == 1)
	{
		std::cerr << "No port provided" << std::endl;
		exit(EXIT_FAILURE);
	}

	// Network arguments.
	const int port = atoi(argv[1]);
	
	auto requestMapper = NetworkTools::RequestMapper();
	auto networkHost = buildNetworkHost(port, requestMapper);
	
	// Declare stuff.
	auto dbContext = DataLayer::BjDatabase();
	
	// Declare blackjack logic.
	auto pointsTools = BlackjackLogic::PointsTools();
	auto playerHandLogic = BlackjackLogic::PlayerHandLogic();
	auto playerLogic = BlackjackLogic::PlayerLogic(
		&playerHandLogic,
		&pointsTools);
	auto dealerLogic = BlackjackLogic::DealerLogic(&pointsTools, &dbContext);
	auto gmStatusLogic = BlackjackLogic::GameStatusLogic(
		&dbContext,
		&playerLogic);
	
	// Services.
	auto logger = Services::Logger();
	auto userManager = Services::UserManager(&dbContext);
	auto sendHelper = Services::SendHelper(
		&networkHost,
		&logger,
		kReceiveMsgHeader);
	auto printHelper = Services::PrintHelper(&pointsTools, 2, "§");

	// Views.
	auto lobbyViews = Views::LobbyViews(&printHelper);
	auto gmSessionViews = Views::GameSessionViews(&printHelper);

	// Declare controllers.
	auto gmSessionController = Controllers::GameSessionController(
		&gmSessionViews,
		&logger,
		&sendHelper,
		&userManager,
		&dbContext,
		&gmStatusLogic,
		&playerLogic,
		&dealerLogic);
	
	auto lobbyController = Controllers::LobbyController(
		&lobbyViews,
		&logger,
		&sendHelper,
		&userManager,
		&dbContext,
		&gmSessionController);
	
	// Map controller's actions.
	addLobbyControllerActions(lobbyController, requestMapper);
	addGmSessionControllerActions(gmSessionController, requestMapper);

	// Start.
	continouslyParseNetworkInput(networkHost);

	return 0;
}
