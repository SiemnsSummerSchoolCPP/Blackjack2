#pragma once

#include "DataLayer/BjDatabase.hpp"

namespace BlackjackLogic
{
	class DealerLogic
	{
	public:
		static const int shoeSize = 5;
	
		DealerLogic(DataLayer::BjDatabase* dbContext);
		
		void init() const;
		PlayingCards::Card& dealCard() const;
		void dealfirstDealersCards() const;
		void dealPlayersCards() const;
		
	private:
		DataLayer::BjDatabase& m_dbContext;
		
		PlayingCards::Shoe& getShoe() const;
	};
}
