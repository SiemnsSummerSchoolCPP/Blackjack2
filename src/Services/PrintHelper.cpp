#include "PrintHelper.hpp"
#include <sstream>
#include <iomanip>

using namespace Services;

PrintHelper::PrintHelper(
	const BlackjackLogic::PointsTools* pointsTools,
	const size_t precision,
	const std::string& moneySign) :
	m_pointsTools(pointsTools),
	m_moneyPrecision(precision),
	m_moneySign(moneySign)
{
}

std::string PrintHelper::formatMoney(const double& money) const
{
	std::stringstream ss;
	
	ss	<< std::fixed << std::setprecision(m_moneyPrecision)
		<< money << m_moneySign;
	return ss.str();
}

std::string PrintHelper::formatHand(const DataLayer::Hand& hand) const
{
	std::stringstream ss;
	
	ss << hand << "(";
	
	const auto handPoints = m_pointsTools->getHandPoints(hand);
	if (m_pointsTools->isBlackjack(hand))
		ss << "Blackjack!";
	else
	{
		if (m_pointsTools->isBusted(handPoints))
			ss << "BUSTED: ";
		ss << handPoints << "p";
	}
	ss << ")";
	return ss.str();
}

std::string PrintHelper::formatPlayerHand(
	const DataLayer::PlayerHand& hand) const
{
	std::stringstream ss;
	
	ss	<< "[ " << formatMoney(hand.bet.amount) << ": "
		<< formatHand(hand)
		<< "]";
	return ss.str();
}

std::string PrintHelper::formatHands(
	const std::vector<DataLayer::PlayerHand*>& hands) const
{
	std::stringstream ss;
	
	for (const auto& hand : hands)
	{
		ss << formatPlayerHand(*hand) << " ";
	}
	return ss.str();
}
			
