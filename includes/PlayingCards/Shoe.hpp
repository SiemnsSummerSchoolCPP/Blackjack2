#pragma once

#include "Deck.hpp"

namespace PlayingCards
{
	class Shoe
	{
	public:
		struct NotEnoughCards : std::exception {};
	
		Shoe(size_t nbOfDecks = 1);
		
		void shuffle();
		CardPtr dealCard();
	
	private:
		std::vector<CardPtr> m_cards;
	};
}
