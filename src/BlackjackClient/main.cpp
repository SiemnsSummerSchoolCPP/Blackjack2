#include "SocketConnection/Exceptions.h"
#include "NetworkTools/RequestMapper.hpp"
#include "NetworkTools/NetworkClient.hpp"
#include "NetworkTools/NetworkClientModel.h"
#include "Controllers/MsgReadController.hpp"
#include "UserInputControllers/LobbyController.hpp"
#include "KeyboardInputTools/KeyboardCmdManager.hpp"
#include <iostream>
#include <thread>

using namespace BlackjackClient;
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



static NetworkTools::NetworkClient buildNetworkClient(
	const char* const ip,
	const int port,
	const NetworkTools::RequestMapper& requestMapper)
{
	

	auto model = NetworkTools::NetworkClientModel();
	model.ip = ip;
	model.port = port;
	model.requestMapper = &requestMapper;
	
	try
	{
		return NetworkTools::NetworkClient(model);
	}
	catch (const SocketConnection::Exceptions::SocketException& e)
	{
		std::cerr << "Setup: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

static void addMsgReaderControllerActions(
	const Controllers::MsgReadController& ctrl,
	NetworkTools::RequestMapper& mapper)
{
	ADD_CONTROLLER_ACTION(
		mapper,
		Controllers::MsgReadController::RequestHeaders::kReceiveMsg,
		ctrl.receiveMsg);
}

static void addUserInLobbyControllerActions(
	const UserInputControllers::LobbyController& ctrl,
	KeyboardInputTools::KeyboardCmdManager& cmdManager)
{
	cmdManager.subscribeParser([&](const std::string input) -> bool
	{
		return ctrl.sendMsg(input);
	});
}

void continuouslyParseUserInput(
	const KeyboardInputTools::KeyboardCmdManager& cmdManager)
{
	while (true)
	{
		cmdManager.parseCmd(
			KeyboardInputTools::KeyboardCmdManager::readKeyboardInput());
	}
}

void continuouslyParseNetworkInput(NetworkTools::NetworkClient& networkClient)
{
	while (true)
	{
		try
		{
			networkClient.parseAnyIncomingActivities();
		}
		catch (const SocketConnection::Exceptions::SocketException& e)
		{
			if (errno == EBADF)
				std::cerr << "Disconnected." << std::endl;
			else
				std::cerr << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}

int main(const int argc, const char* const* const argv)
{
	if (argc < 3)
	{
		std::cerr << "No ip or/and port provided." << std::endl;
		exit(EXIT_FAILURE);
	}

	// Network arguments.
	const auto ip = argv[1];
	const auto port = atoi(argv[2]);
	
	auto requestMapper = NetworkTools::RequestMapper();
	auto cmdManager = KeyboardInputTools::KeyboardCmdManager();
	auto networkClient = buildNetworkClient(ip, port, requestMapper);
	
	// Declare controllers.
	auto msgReaderController = Controllers::MsgReadController();
	const auto userInputLobbyController =
		UserInputControllers::LobbyController(networkClient);
	
	// Map controller's actions.
	addMsgReaderControllerActions(msgReaderController, requestMapper);
	addUserInLobbyControllerActions(userInputLobbyController, cmdManager);
	
	// Start.
	{
		auto networkWatcher = std::thread([&]()
		{
			continuouslyParseNetworkInput(networkClient);
		});
		continuouslyParseUserInput(cmdManager);
		
		networkWatcher.join();
	}
	return 0;
}
