#pragma once

#include "Hand.h"
#include "Bet.h"

namespace DataLayer
{
	struct PlayerHand : public Hand
	{
		enum class State
		{
			kSitting,
			kStanding
		};
		
		Bet bet;
		PlayerHand::State state = PlayerHand::State::kSitting;
	};
}
