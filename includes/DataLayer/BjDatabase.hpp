#pragma once

#include "UserModel.h"
#include "PlayerModel.h"
#include "SocketConnection/Connection.h"
#include "GameSession.h"
#include <vector>
#include <list>
#include <unordered_map>

namespace DataLayer
{
	class BjDatabase
	{
	public:
		BjDatabase();
	
		std::unordered_map<int, UserModel*>& getUsers();
		std::unordered_map<int, PlayerModel*>& getPlayers();
		GameSession& getGameSession();
		
		UserModel* getUser(const SocketConnection::Connection& connection);
		
	private:
		std::unordered_map<int, UserModel*> m_users;
		std::unordered_map<int, PlayerModel*> m_players;
		GameSession m_gameSession;
	};
}
