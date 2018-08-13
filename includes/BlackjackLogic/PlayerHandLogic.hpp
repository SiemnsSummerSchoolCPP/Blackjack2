#pragma once

#include "DataLayer/PlayerHand.h"

namespace BlackjackLogic
{
	class PlayerHandLogic
	{
	public:
		bool canHit(const DataLayer::PlayerHand& hand) const;
		bool aBetIsPlaced(const DataLayer::PlayerHand& hand) const;
	};
}
