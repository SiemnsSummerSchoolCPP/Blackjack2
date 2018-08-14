#pragma once

#include "PointsTools.hpp"
#include "DataLayer/BjDatabase.hpp"

namespace BlackjackLogic
{
	class DealerLogic
	{
	public:
		static const int shoeSize = 5;
	
		DealerLogic(
			const PointsTools& pointsTools,
			DataLayer::BjDatabase* dbContext);
		
		void init() const;
		void endTheGame() const;
		
		int dealersHandPoints() const;
		PlayingCards::Card& dealCard() const;
		
		void dealFirstDealersCards() const;
		void dealFinalDealersCards() const;
		void dealPlayersCards() const;
		
	private:
		const PointsTools& m_pointsTools;
		DataLayer::BjDatabase& m_dbContext;
		
		PlayingCards::Shoe& getShoe() const;
	};
}
