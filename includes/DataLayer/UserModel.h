#pragma once

#include "GameJoinState.h"
#include <string>
#include <memory>

namespace DataLayer
{
	struct UserModel
	{
		int uniqueId = 0;
		std::string name;
		double money = 0;
		GameJoinState joinState;
	};
	
	typedef std::unique_ptr<UserModel> UserModelPtr;
}
