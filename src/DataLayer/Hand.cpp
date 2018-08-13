#include "Hand.h"

using namespace DataLayer;

std::ostream& DataLayer::operator << (std::ostream& o, const Hand& hand)
{
	for (const auto& card : hand.cards)
	{
		o << *card << " ";
	}
	return o;
}
