#include "DealerLogic.hpp"

using namespace BlackjackLogic;

DealerLogic::DealerLogic(
	const PointsTools* pointsTools,
	DataLayer::BjDatabase* dbContext) :
	m_pointsTools(pointsTools),
	m_dbContext(dbContext)
{
}

void DealerLogic::init() const
{
	m_dbContext->gameSession.shoe.reset(new PlayingCards::Shoe(shoeSize));
	m_dbContext->gameSession.dealersHand.reset(new DataLayer::Hand());
}

void DealerLogic::endTheGame() const
{
}

PlayingCards::CardPtr DealerLogic::dealCard() const
{
	return getShoe().dealCard();
}

void DealerLogic::dealFirstDealersCards() const
{
	auto& dealersHand = *m_dbContext->gameSession.dealersHand;
	
	for (int i = 0; i < 2; i++)
	{
		dealersHand.cards.push_back(dealCard());
	}
}

void DealerLogic::dealFinalDealersCards() const
{
	while (dealersHandPoints() < m_pointsTools->dealerPointsLimit)
	{
		m_dbContext->gameSession.dealersHand->cards.push_back(dealCard());
	}
}

void DealerLogic::dealPlayersCards() const
{
	for (const auto& pair : m_dbContext->players)
	{
		const auto& player = *pair.second;
		
		for (int i = 0; i < 2; i++)
		{
			player.hands[0]->cards.push_back(dealCard());
		}
	}
}

/*
** Private helpers.
*/

PlayingCards::Shoe& DealerLogic::getShoe() const
{
	return *m_dbContext->gameSession.shoe;
}

int DealerLogic::dealersHandPoints() const
{
	return
		m_pointsTools->getHandPoints(*m_dbContext->gameSession.dealersHand);
}
