#include "PointsTools.hpp"
#include <unordered_map>

using namespace BlackjackLogic;

const PointsTools::CardPointsMap PointsTools::cardPoints =
{
	{ PlayingCards::Card::Rank::kTwo,	2 },
	{ PlayingCards::Card::Rank::kThree,	3 },
	{ PlayingCards::Card::Rank::kFour,	4 },
	{ PlayingCards::Card::Rank::kFive,	5 },
	{ PlayingCards::Card::Rank::kSix,	6 },
	{ PlayingCards::Card::Rank::kSeven,	7 },
	{ PlayingCards::Card::Rank::kEight,	8 },
	{ PlayingCards::Card::Rank::kNine,	9 },
	{ PlayingCards::Card::Rank::kTen,	10 },
	
	{ PlayingCards::Card::Rank::kJack,	10 },
	{ PlayingCards::Card::Rank::kQueen,	10 },
	{ PlayingCards::Card::Rank::kKing,	10 },
	
	{ PlayingCards::Card::Rank::kAce,	11 },
};

int PointsTools::getCardPoints(const PlayingCards::Card& card) const
{
	return cardPoints.at(card.getRank());
}

int PointsTools::getHandPoints(
	const DataLayer::Hand& hand,
	const bool isSmart) const
{
	int result = 0;
	
	for (const auto& card : hand.cards)
	{
		result += getCardPoints(*card);
	}
	
	if (!isBusted(result) || !isSmart)
		return result;
	
	// Smart ace.
	for (const auto& card : hand.cards)
	{
		if (isBusted(result))
			break;
		if (card->getRank() == PlayingCards::Card::Rank::kAce)
		{
			result -= getCardPoints(*card);
			result += 1;
		}
	}
	
	return result;
}

bool PointsTools::isBusted(const int points) const
{
	return points > PointsTools::blackjackPoints;
}

bool PointsTools::isBusted(const DataLayer::Hand& hand) const
{
	return isBusted(getHandPoints(hand));
}

bool PointsTools::isBlackjack(const DataLayer::Hand& hand) const
{
	return
		(hand.cards.size() == 2) &&
		(getHandPoints(hand) == PointsTools::blackjackPoints);
}
