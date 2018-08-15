#include "Shoe.hpp"
#include <algorithm>
#include <assert.h>

using namespace PlayingCards;

Shoe::Shoe(size_t nbOfDecks)
{
	for (int i = 0; i < nbOfDecks; i++)
	{
		auto cards = Deck::freshDeck();
		m_cards.insert(m_cards.end(), cards, cards + Deck::nbOfCards);
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

Card& Shoe::dealCard()
{
	if (m_cards.size() == 0)
		throw NotEnoughCards();

	auto& card = *m_cards.back();
	m_cards.pop_back();
	return card;
}
