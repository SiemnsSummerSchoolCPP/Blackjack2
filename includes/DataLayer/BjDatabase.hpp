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
	struct BjDatabase
	{
		std::unordered_map<int, UserModelPtr> users;
		std::unordered_map<int, PlayerModelPtr> players;
		GameSession gameSession;
		
		UserModel* getUser(
			const SocketConnection::Connection& connection) const;
	};
}
