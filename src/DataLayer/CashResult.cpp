#include "CashResult.h"

using namespace DataLayer;

std::ostream& DataLayer::operator << (
	std::ostream& o,
	const CashResult::State& state)
{
	switch (state)
	{
		case (DataLayer::CashResult::State::kWin):
			o << "Win";
			break;
		case (DataLayer::CashResult::State::kLost):
			o << "Lost";
			break;
		case (DataLayer::CashResult::State::kDraw):
			o << "Draw";
			break;
		case (DataLayer::CashResult::State::kBlackjack):
			o << "BLACKJACK!";
			break;
	}
	return o;
}
