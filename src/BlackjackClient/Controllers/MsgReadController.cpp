#include "Controllers/MsgReadController.hpp"
#include <iostream>

using namespace BlackjackClient::Controllers;

int MsgReadController::receiveMsg(
	const SocketConnection::Connection&,
	const Requests::Request& request) const
{
	std::cout << reinterpret_cast<const char*>(request.data) << std::endl;
	return 0;
}
