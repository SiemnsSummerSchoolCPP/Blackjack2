#pragma once

#include "DataLayer/BjDatabase.hpp"
#include "BlackjackLogic/PointsTools.hpp"
#include "BlackjackLogic/PlayerHandLogic.hpp"
#include "BlackjackLogic/PlayerLogic.hpp"
#include "BlackjackLogic/DealerLogic.hpp"
#include "BlackjackLogic/GameStatusLogic.hpp"

namespace BlackjackServer
{
	struct AppLogic
	{
		AppLogic(DataLayer::BjDatabase* dbContext);
		
		BlackjackLogic::PointsTools pointsTools;
		BlackjackLogic::PlayerHandLogic playerHandLogic;
		BlackjackLogic::PlayerLogic playerLogic;
		BlackjackLogic::DealerLogic dealerLogic;
		BlackjackLogic::GameStatusLogic gameStatusLogic;
	};
}
