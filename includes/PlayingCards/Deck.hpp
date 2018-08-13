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
		~Deck();

		static Card** freshDeck();

		bool isEmpty() const;
		void shuffle();
		Card& dealCard();

	private:
		Card** m_cards = nullptr;
		int m_cardIndex = 0;
	};
	
	struct Deck::NotEnoughCards : public std::exception
	{
		const char* what() const throw();
	};
}
