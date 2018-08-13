#pragma once

#include "PlayingCards/Shoe.hpp"
#include "Hand.h"

namespace DataLayer
{
	struct GameSession
	{
		enum class State
		{
			kNotStarted,
			kBettingPhase,
			kHitStandPhase,
			kCashingPhase
		};
		
		GameSession::State state = State::kNotStarted;
		
		PlayingCards::Shoe* shoe = nullptr;
		Hand* dealersHand = nullptr;
	};
}
