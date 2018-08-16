#include "Deck.hpp"
#include <assert.h>

using namespace PlayingCards;

/*
** Exceptions
*/

const char* Deck::NotEnoughCards::what() const throw()
{
	return "Not enough cards in deck.";
}

/*
** Cosntructors & destructors.
*/

Deck::Deck() : m_cards(freshDeck())
{
}

/*
** Public methods
*/

bool Deck::isEmpty() const
{
	return m_cardIndex >= Deck::nbOfCards;
}

void Deck::shuffle()
{
	std::random_device randomDevice;
	std::mt19937 merseneTwister(randomDevice());
	std::uniform_int_distribution<> uniformDistrib(0, Deck::nbOfCards - 1);
	
	for (auto i = 0; i < Deck::nbOfCards; i++)
	{
		auto index1 = uniformDistrib(merseneTwister) % Deck::nbOfCards;
		auto index2 = uniformDistrib(merseneTwister) % Deck::nbOfCards;
		std::swap(m_cards[index1], m_cards[index2]);
	}
	m_cardIndex = 0;
}

Card& Deck::dealCard()
{
	if (m_cardIndex >= Deck::nbOfCards)
	{
		throw NotEnoughCards();
	}
	
	return *m_cards[m_cardIndex++];
}

/*
** Private static methods
*/

static const Card::Suit suits[] =
{
	Card::Suit::kDiamonds,
	Card::Suit::kSpades,
	Card::Suit::kClubs,
	Card::Suit::kHearts,
};

std::vector<Card*> Deck::freshDeck()
{
	auto cards = std::vector<Card*>();
	
	const auto minRank = static_cast<int>(
		std::numeric_limits<Card::Rank>::min());
	const auto maxRank = static_cast<int>(
		std::numeric_limits<Card::Rank>::max());

	cards.reserve(Deck::nbOfCards);
	for (const auto& suit : suits)
	{
		for (auto rank = minRank; rank <= maxRank; rank++)
		{
			cards.push_back(new Card(
				static_cast<Card::Suit>(suit),
				static_cast<Card::Rank>(rank)
			));
		}
	}
	
	assert(cards.size() == Deck::nbOfCards);
	return cards;
}
