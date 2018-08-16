#pragma once

#include "Hand.h"
#include "Bet.h"
#include <ostream>
#include <memory>

namespace DataLayer
{
	struct PlayerHand : public Hand
	{
		enum State
		{
			kSitting	= 1 << 1,
			kStanding	= 1 << 2
		};
		
		Bet bet;
		PlayerHand::State state = PlayerHand::State::kSitting;
	};
	
	typedef std::unique_ptr<PlayerHand> PlayerHandPtr;
}
