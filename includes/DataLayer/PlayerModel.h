#pragma once

#include "UserModel.h"
#include "PlayerHand.h"
#include <vector>

namespace DataLayer
{
	struct PlayerModel
	{
		UserModel* userModel;
		std::vector<PlayerHand> hands;
	};
}
