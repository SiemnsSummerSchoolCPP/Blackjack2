#pragma once

#include "UserModel.h"
#include "PlayerModel.h"
#include <vector>

namespace DataLayer
{
	class BjDatabase
	{
	public:
		BjDatabase();
	
	private:
		std::vector<UserModel> m_users;
		std::vector<PlayerModel> m_players;
	};
}
