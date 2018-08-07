#pragma once

#include "GameJoinState.h"
#include <string>

namespace DataLayer
{
	struct UserModel
	{
		int uniqueId = 0;
		std::string name;
		double money = 0;
		GameJoinState joinState;
	};
}
