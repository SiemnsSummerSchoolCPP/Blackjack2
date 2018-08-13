#include "PlayerHandLogic.hpp"

using namespace BlackjackLogic;

bool PlayerHandLogic::canHit(const DataLayer::PlayerHand& hand) const
{
	return hand.state & DataLayer::PlayerHand::State::kSitting;
}

bool PlayerHandLogic::aBetIsPlaced(const DataLayer::PlayerHand& hand) const
{
	return hand.bet.amount > std::numeric_limits<double>::epsilon();
}
