#pragma once

#include "Card.hpp"
#include <array>
#include <random>

namespace PlayingCards
{
	class Deck
	{
	public:
		static const int nbOfCards = 52;
		struct NotEnoughCards;
	
		Deck();

		static std::vector<Card*> freshDeck();

		bool isEmpty() const;
		void shuffle();
		Card& dealCard();

	private:
		std::vector<Card*> m_cards;
		int m_cardIndex = 0;
	};
	
	struct Deck::NotEnoughCards : public std::exception
	{
		const char* what() const throw();
	};
}
