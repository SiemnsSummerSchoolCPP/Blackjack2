#pragma once

#include "GameJoinState.h"
#include <string>

namespace DataLayer
{
	struct UserModel
	{
		std::string name;
		double money;
		GameJoinState joinState;
	};
}
