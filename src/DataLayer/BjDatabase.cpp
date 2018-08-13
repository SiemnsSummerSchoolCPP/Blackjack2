#include "BjDatabase.hpp"
#include "Hand.h"
#include "PlayerHand.h"
#include <algorithm>

using namespace DataLayer;

BjDatabase::BjDatabase()
{
}

std::unordered_map<int, UserModel*>& BjDatabase::getUsers()
{
	return m_users;
}

std::unordered_map<int, PlayerModel*>& BjDatabase::getPlayers()
{
	return m_players;
}

GameSession& BjDatabase::getGameSession()
{
	return m_gameSession;
}

UserModel* BjDatabase::getUser(const SocketConnection::Connection& connection)
{
	if (m_users.find(connection.socket) == m_users.end())
		throw "No such connection in database.";
	
	return m_users[connection.socket];
}

