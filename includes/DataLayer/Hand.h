#pragma once

#include "PlayingCards/Card.hpp"
#include <vector>
#include <ostream>

namespace DataLayer
{
	struct Hand
	{
		std::vector<const PlayingCards::Card*> cards;
	};
	
	std::ostream& operator << (std::ostream& o, const Hand& hand);
}
