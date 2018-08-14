#pragma once

#include "PlayingCards/Card.hpp"
#include "DataLayer/Hand.h"
#include <unordered_map>

namespace BlackjackLogic
{
	class PointsTools
	{
	public:
		static const int dealerPointsLimit = 17;
		constexpr static double blackjackWinMultiplier = 3.0 / 2;
		constexpr static double simplekWinMultiplier = 1.0;
	
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
