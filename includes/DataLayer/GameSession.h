#pragma once

#include "PlayingCards/Shoe.hpp"
#include "Hand.h"
#include <memory>

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
		
		std::unique_ptr<PlayingCards::Shoe> shoe;
		std::unique_ptr<Hand> dealersHand;
	};
}
