#pragma once

#include "Bet.h"
#include <ostream>

namespace DataLayer
{
	struct CashResult
	{
		enum class State
		{
			kWin,
			kBlackjack,
			kDraw,
			kLost
		};
		
		CashResult::State state;
		
		const Bet* bet;
		double receivedMoney = 0.0;
		double income = 0.0;
		double winMultipiler = 0.0;
	};
	
	std::ostream& operator << (std::ostream& o, const CashResult::State& state);
}
