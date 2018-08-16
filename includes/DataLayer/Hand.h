#pragma once

#include "PlayingCards/Card.hpp"
#include <vector>
#include <ostream>

namespace DataLayer
{
	struct Hand
	{
		std::vector<PlayingCards::CardPtr> cards;
	};
	
	std::ostream& operator << (std::ostream& o, const Hand& hand);
}
