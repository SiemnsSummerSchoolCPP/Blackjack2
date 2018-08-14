#pragma once

#include "PlayerLogic.hpp"
#include "DealerLogic.hpp"
#include "DataLayer/BjDatabase.hpp"
#include "DataLayer/GameSession.h"

namespace BlackjackLogic
{
	class GameStatusLogic
	{
	public:
		GameStatusLogic(
			DataLayer::BjDatabase& dbContext,
			const PlayerLogic& playerLogic);
	
		bool allPlayersHavePlacedTheirBets() const;
		bool allHandsAreStanding() const;
		
	private:
		DataLayer::BjDatabase& m_dbContext;
		const PlayerLogic& m_playerLogic;
	};
}
