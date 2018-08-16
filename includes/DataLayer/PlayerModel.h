#pragma once

#include "UserModel.h"
#include "PlayerHand.h"
#include <vector>
#include <memory>

namespace DataLayer
{
	struct PlayerModel
	{
		UserModel* userModel;
		std::vector<PlayerHandPtr> hands;
	};
	
	typedef std::unique_ptr<PlayerModel> PlayerModelPtr;
}
