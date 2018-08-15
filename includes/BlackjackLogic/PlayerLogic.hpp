#pragma once

#include "PlayerHandLogic.hpp"
#include "PointsTools.hpp"
#include "DataLayer/PlayerModel.h"
#include "DataLayer/BjDatabase.hpp"
#include "DataLayer/CashResult.h"

namespace BlackjackLogic
{
	class PlayerLogic
	{
	public:
		struct PlayerLogicException : public std::exception {};
		struct NoSuchHand : public PlayerLogicException {};
		struct HandIsNotSitting : public PlayerLogicException {};
		struct InvalidBet : public PlayerLogicException {};
		struct NotEnoughMoney : public PlayerLogicException {};
		struct AlreadyPlacedABet : public PlayerLogicException {};
	
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
		
		DataLayer::CashResult cash(
			const DataLayer::PlayerHand&,
			int dealersPoints) const;
		
	private:
		const PlayerHandLogic& m_handLogic;
		const PointsTools& m_pointsTools;
	};
}
