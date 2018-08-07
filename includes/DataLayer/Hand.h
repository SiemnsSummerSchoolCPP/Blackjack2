#pragma once

#include "PlayingCards/Card.hpp"
#include <vector>

namespace DataLayer
{
	struct Hand
	{
		std::vector<PlayingCards::Card*> cards;
	};
}
