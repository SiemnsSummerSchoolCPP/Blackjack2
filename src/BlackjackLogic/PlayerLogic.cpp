#include "PlayerLogic.hpp"

using namespace BlackjackLogic;

PlayerLogic::PlayerLogic(
	const PlayerHandLogic& handLogic,
	const PointsTools& pointsTools) :
	m_handLogic(handLogic),
	m_pointsTools(pointsTools)
{
}

bool PlayerLogic::canHit(const DataLayer::PlayerModel& player) const
{
	for (const auto& hand : player.hands)
	{
		if (m_handLogic.canHit(*hand))
			return true;
	}
	
	return false;
}

bool PlayerLogic::hasPlacedHisBets(const DataLayer::PlayerModel& player) const
{
	for (const auto& hand : player.hands)
	{
		if (!m_handLogic.aBetIsPlaced(*hand))
			return false;
	}
	return true;
}

bool PlayerLogic::allHandsAreStanding(const DataLayer::PlayerModel& player) const
{
	return !std::any_of(
		player.hands.begin(),
		player.hands.end(),
		[&](const auto& hand)
		{
			return !(hand->state & DataLayer::PlayerHand::State::kStanding);
		});
}

void PlayerLogic::executeBet(
	DataLayer::PlayerModel& player,
	const DataLayer::Bet& bet) const
{
	if (bet.handIndex >= player.hands.size())
		throw NoSuchHand();
	
	auto& hand = *player.hands[bet.handIndex];
	if (!(hand.state & DataLayer::PlayerHand::State::kSitting))
		throw HandIsNotSitting();
	
	if (bet.amount <= std::numeric_limits<double>::epsilon())
		throw InvalidBet();
	
	const auto& money = player.userModel->money;
	if (bet.amount > money + std::numeric_limits<double>::epsilon())
		throw NotEnoughMoney();
	
	if (hand.bet.amount > std::numeric_limits<double>::epsilon())
		throw AlreadyPlacedABet();
	
	player.userModel->money -= bet.amount;
	hand.bet.amount += bet.amount;
}

const PlayingCards::Card* PlayerLogic::executeHit(
	DataLayer::PlayerModel& player,
	const int handIndex,
	std::function<const PlayingCards::Card& ()> newCardF) const
{
	if (handIndex >= player.hands.size())
		throw NoSuchHand();
	
	auto& hand = *player.hands[handIndex];
	if (!(hand.state & DataLayer::PlayerHand::State::kSitting))
		throw HandIsNotSitting();

	const auto& newCard = &newCardF();
	hand.cards.push_back(newCard);
	if (m_pointsTools.isBusted(hand) || m_pointsTools.isBlackjack(hand))
	{
		hand.state = DataLayer::PlayerHand::State::kStanding;
	}
	return newCard;
}

void PlayerLogic::executeStand(
	DataLayer::PlayerModel& player,
	const int handIndex) const
{
	if (handIndex >= player.hands.size())
		throw NoSuchHand();
	
	auto& hand = *player.hands[handIndex];
	if (!(hand.state & DataLayer::PlayerHand::State::kSitting))
		throw HandIsNotSitting();
	
	hand.state = DataLayer::PlayerHand::State::kStanding;
}
