#pragma once

#include "PlayerHandLogic.hpp"
#include "PointsTools.hpp"
#include "DataLayer/PlayerModel.h"
#include "DataLayer/BjDatabase.hpp"

namespace BlackjackLogic
{
	class PlayerLogic
	{
	public:
		struct NoSuchHand : public std::exception {};
		struct HandIsNotSitting : public std::exception {};
		struct InvalidBet : public std::exception {};
		struct NotEnoughMoney : public std::exception {};
		struct AlreadyPlacedABet : public std::exception {};
	
		PlayerLogic(
			const PlayerHandLogic& handLogic,
			const PointsTools& pointsTools);
	
		bool canHit(const DataLayer::PlayerModel& player) const;
		bool hasPlacedHisBets(const DataLayer::PlayerModel& player) const;
		bool allHandsAreStanding(const DataLayer::PlayerModel& player) const;
		
		void executeBet(
			DataLayer::PlayerModel& player,
			const DataLayer::Bet& bet) const;
		
		const PlayingCards::Card* executeHit(
			DataLayer::PlayerModel& player,
			int handIndex,
			std::function<const PlayingCards::Card& ()> newCardF) const;
		
		void executeStand(
			DataLayer::PlayerModel& player,
			int handIndex) const;
		
	private:
		const PlayerHandLogic& m_handLogic;
		const PointsTools& m_pointsTools;
	};
}
