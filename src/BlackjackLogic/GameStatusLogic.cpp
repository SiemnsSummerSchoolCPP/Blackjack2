#include "GameStatusLogic.hpp"

using namespace BlackjackLogic;
using namespace DataLayer;

GameStatusLogic::GameStatusLogic(
	DataLayer::BjDatabase& dbContext,
	const PlayerLogic& playerLogic) :
	m_dbContext(dbContext),
	m_playerLogic(playerLogic)
{
}

bool GameStatusLogic::allPlayersHavePlacedTheirBets() const
{
	return !std::any_of(
		m_dbContext.getPlayers().cbegin(),
		m_dbContext.getPlayers().cend(),
		[&](const auto& pair)
		{
			return !m_playerLogic.hasPlacedHisBets(*pair.second);
		});
}

bool GameStatusLogic::allHandsAreStanding() const
{
	return !std::any_of(
		m_dbContext.getPlayers().cbegin(),
		m_dbContext.getPlayers().cend(),
		[&](const auto& pair)
		{
			const auto& player = *pair.second;
			return !m_playerLogic.allHandsAreStanding(player);
		});
}
