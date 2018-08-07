#include "SocketConnection/Exceptions.h"
#include "NetworkTools/RequestMapper.hpp"
#include "NetworkTools/NetworkClient.hpp"
#include "NetworkTools/NetworkClientModel.h"
#include "Controllers/MsgReadController.hpp"
#include "BlackjackServer/Controllers/LobbyController.hpp"
#include <iostream>

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

int main(const int argc, const char* const* const argv)
{
	if (argc < 3)
	{
		std::cerr << "No ip or/and port provided." << std::endl;
		exit(EXIT_FAILURE);
	}

	const auto ip = argv[1];
	const auto port = atoi(argv[2]);
	
	auto requestMapper = NetworkTools::RequestMapper();
	auto networkClient = buildNetworkClient(ip, port, requestMapper);
	
	auto msgReaderController = MsgReadController();
	ADD_CONTROLLER_ACTION(
		requestMapper,
		MsgReadController::RequestHeaders::kReceiveMsg,
		msgReaderController.receiveMsg);
	
	Requests::Request request(
		BlackjackServer::LobbyController::RequestHeaders::kSendMsg,
		"hello");
	networkClient.sendRequest(request);
	
	while (true)
	{
		try
		{
			networkClient.parseAnyIncomingActivities();
		}
		catch (const SocketConnection::Exceptions::SocketException& e)
		{
			std::cerr << e.what() << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}
