#include "Shoe.hpp"
#include <algorithm>
#include <assert.h>

using namespace PlayingCards;

Shoe::Shoe(size_t nbOfDecks)
{
	for (size_t i = 0; i < nbOfDecks; i++)
	{
		auto cards = Deck::freshDeck();
		for (int i = 0; i < Deck::nbOfCards; i++)
		{
			auto cardPtr = CardPtr(cards[i]);
			m_cards.push_back(std::move(cardPtr));
		}
	}
	assert(m_cards.size() == nbOfDecks * Deck::nbOfCards);
	shuffle();
}

void Shoe::shuffle()
{
	std::random_device randomDevice;
	std::mt19937 merseneTwister(randomDevice());
	
	std::shuffle(m_cards.begin(), m_cards.end(), merseneTwister);
}

CardPtr Shoe::dealCard()
{
	if (m_cards.size() == 0)
		throw NotEnoughCards();

	auto card = std::move(m_cards.back());
	m_cards.pop_back();
	return card;
}
