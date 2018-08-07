#include "Controllers/MsgReadController.hpp"
#include <iostream>

using namespace BlackjackClient;

int MsgReadController::receiveMsg(
	const SocketConnection::Connection& connection,
	const Requests::Request& request) const
{
	std::cout << reinterpret_cast<const char*>(request.data) << std::endl;
	return 0;
}
