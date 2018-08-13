#pragma once

#include "PlayerLogic.hpp"
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
		
		void setBettingPhase();
		void setHitStandPhase();
		void setCashingPhase();
		
	private:
		DataLayer::BjDatabase& m_dbContext;
		const PlayerLogic& m_playerLogic;
	};
}
