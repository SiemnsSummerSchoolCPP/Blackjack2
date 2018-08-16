#pragma once

#include <ostream>
#include <limits>
#include <memory>

namespace PlayingCards
{
	class Card
	{
	public:
		enum class Suit;
		enum class Rank;
		
		Card::Suit getSuit() const;
		Card::Rank getRank() const;
		
		Card(Card::Suit suit, Card::Rank rank);
		
		friend std::ostream& operator << (
			std::ostream& o,
			const Card& target
		);
	
	private:
		const Card::Suit m_suit;
		const Card::Rank m_rank;
		
		const char* suitStr() const;
		const char* rankStr() const;
	};
	
	typedef std::unique_ptr<Card> CardPtr;
	
	/*
	** Body of Enums.
	*/
	
	enum class Card::Suit
	{
		kDiamonds = 0,
		kClubs,
		kHearts,
		kSpades
	};
	
	enum class Card::Rank
	{
		kTwo = 0,
		kThree,
		kFour,
		kFive,
		kSix,
		kSeven,
		kEight,
		kNine,
		kTen,
		kJack,
		kQueen,
		kKing,
		kAce
	};
}

template<> class std::numeric_limits<PlayingCards::Card::Rank>
{
public:
	static PlayingCards::Card::Rank min();
	static PlayingCards::Card::Rank max();
};

namespace std
{
	template <>
	struct hash<PlayingCards::Card::Rank>
	{
		std::size_t operator()(const PlayingCards::Card::Rank& k) const
		{
			return hash<int>()(static_cast<int>(k));
		}
	};
}
