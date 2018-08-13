#pragma once

#include "PlayingCards/Card.hpp"
#include "DataLayer/Hand.h"
#include <unordered_map>

namespace BlackjackLogic
{
	class PointsTools
	{
	public:
		int getCardPoints(const PlayingCards::Card& card) const;
		int getHandPoints(
			const DataLayer::Hand& hand,
			bool isSmart = true) const;
		
		bool isBusted(int points) const;
		bool isBusted(const DataLayer::Hand& hand) const;
		bool isBlackjack(const DataLayer::Hand& hand) const;
		
	private:
		typedef std::unordered_map<PlayingCards::Card::Rank, int> CardPointsMap;
	
		static const int blackjackPoints = 21;
		static const CardPointsMap cardPoints;
	};
}
