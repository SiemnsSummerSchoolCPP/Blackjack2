#include "NetworkTools/RequestMapper.hpp"
#include "NetworkTools/NetworkHost.hpp"
#include "NetworkTools/NetworkHostModel.h"
#include "Controllers/LobbyController.hpp"
#include "SocketConnection/Exceptions.h"
#include "Services/Logger.hpp"
#include "DataLayer/BjDatabase.hpp"
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
	auto lobbyViews = Views::LobbyViews();
	auto dbContext = DataLayer::BjDatabase();
	
	// Services.
	auto logger = Services::Logger();
	
	// Declare controllers.
	auto lobbyController = Controllers::LobbyController(
		networkHost,
		lobbyViews,
		logger,
		dbContext);
	
	// Map controller's actions.
	addLobbyControllerActions(lobbyController, requestMapper);

	// Start.
	continouslyParseNetworkInput(networkHost);

	return 0;
}
