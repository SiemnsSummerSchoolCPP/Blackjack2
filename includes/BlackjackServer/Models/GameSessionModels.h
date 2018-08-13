#pragma once

#include "DataLayer/UserModel.h"
#include "DataLayer/PlayerModel.h"
#include "DataLayer/Bet.h"
#include <vector>
#include <string>

namespace BlackjackServer { namespace Models { namespace GmSessionModels
{
	struct StartGameModel
	{
		const std::vector<DataLayer::UserModel*>* joinedUsers;
	};
	
	struct InitialHandsSetupModel
	{
		const PlayingCards::Card* dealersCard;
		const std::vector<DataLayer::PlayerModel*>* players;
	};
	
	struct SuccessfulBetModel
	{
		const DataLayer::PlayerModel* player;
		const DataLayer::Bet* bet;
	};
	
	struct SuccessfulHitModel
	{
		const PlayingCards::Card* cardDealt;
		const DataLayer::PlayerModel* player;
	};
	
	struct SuccessfulStandModel
	{
		const DataLayer::PlayerModel* player;
	};
}}}
