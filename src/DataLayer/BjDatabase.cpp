#include "BjDatabase.hpp"
#include "Hand.h"
#include "PlayerHand.h"
#include <algorithm>

using namespace DataLayer;

UserModel* BjDatabase::getUser(
	const SocketConnection::Connection& connection) const
{
	if (users.find(connection.socket) == users.end())
		throw "No such connection in database.";
	
	return users.at(connection.socket).get();
}

