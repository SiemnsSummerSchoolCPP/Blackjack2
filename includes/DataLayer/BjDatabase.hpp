#pragma once

#include "UserModel.h"
#include "PlayerModel.h"
#include "SocketConnection/Connection.h"
#include <vector>

namespace DataLayer
{
	class BjDatabase
	{
	public:
		BjDatabase();
	
		std::vector<UserModel*>& getUsers();
		std::vector<PlayerModel*>& getPlayers();
		
		UserModel* getUser(const SocketConnection::Connection& connection);
		
	private:
		std::vector<UserModel*> m_users;
		std::vector<PlayerModel*> m_players;
	};
}
