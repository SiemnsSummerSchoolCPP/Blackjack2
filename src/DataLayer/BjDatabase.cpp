#include "BjDatabase.hpp"
#include "Hand.h"
#include "PlayerHand.h"
#include <algorithm>

using namespace DataLayer;

BjDatabase::BjDatabase()
{
}

std::vector<UserModel*>& BjDatabase::getUsers()
{
	return m_users;
}

std::vector<PlayerModel*>& BjDatabase::getPlayers()
{
	return m_players;
}

UserModel* BjDatabase::getUser(const SocketConnection::Connection& connection)
{
	auto findResult = std::find_if(
		m_users.begin(),
		m_users.end(),
		[&](const UserModel* user)
		{
			return user->uniqueId == connection.socket;
		});
	
	if (findResult == m_users.end())
		throw "No such connection in database.";
	
	return *findResult;
}
