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
	m_dbContext->getGameSession().shoe = new PlayingCards::Shoe(shoeSize);
	m_dbContext->getGameSession().dealersHand = new DataLayer::Hand();
}

void DealerLogic::endTheGame() const
{
//	if (m_dbContext->getGameSession().dealersHand)
//		delete m_dbContext->getGameSession().dealersHand;
//	
//	if (m_dbContext->getGameSession().shoe)
//		delete m_dbContext->getGameSession().shoe;
}

PlayingCards::Card& DealerLogic::dealCard() const
{
	return getShoe().dealCard();
}

void DealerLogic::dealFirstDealersCards() const
{
	auto& dealersHand = *m_dbContext->getGameSession().dealersHand;
	
	for (int i = 0; i < 2; i++)
	{
		dealersHand.cards.push_back(&dealCard());
	}
}

void DealerLogic::dealFinalDealersCards() const
{
	while (dealersHandPoints() < m_pointsTools->dealerPointsLimit)
	{
		m_dbContext->getGameSession().dealersHand->cards.push_back(&dealCard());
	}
}

void DealerLogic::dealPlayersCards() const
{
	for (const auto& pair : m_dbContext->getPlayers())
	{
		const auto& player = *pair.second;
		
		for (int i = 0; i < 2; i++)
		{
			player.hands[0]->cards.push_back(&dealCard());
		}
	}
}

/*
** Private helpers.
*/

PlayingCards::Shoe& DealerLogic::getShoe() const
{
	return *m_dbContext->getGameSession().shoe;
}

int DealerLogic::dealersHandPoints() const
{
	return
		m_pointsTools->getHandPoints(*m_dbContext->getGameSession().dealersHand);
}
