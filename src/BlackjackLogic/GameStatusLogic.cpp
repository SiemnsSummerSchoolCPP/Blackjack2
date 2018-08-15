#include "GameStatusLogic.hpp"
#include <algorithm>

using namespace BlackjackLogic;
using namespace DataLayer;

GameStatusLogic::GameStatusLogic(
	const DataLayer::BjDatabase* dbContext,
	const PlayerLogic* playerLogic) :
	m_dbContext(dbContext),
	m_playerLogic(playerLogic)
{
}

bool GameStatusLogic::allPlayersHavePlacedTheirBets() const
{
	return !std::any_of(
		m_dbContext->players.cbegin(),
		m_dbContext->players.cend(),
		[&](const auto& pair)
		{
			return !m_playerLogic->hasPlacedHisBets(*pair.second);
		});
}

bool GameStatusLogic::allHandsAreStanding() const
{
	return !std::any_of(
		m_dbContext->players.cbegin(),
		m_dbContext->players.cend(),
		[&](const auto& pair)
		{
			const auto& player = *pair.second;
			return !m_playerLogic->allHandsAreStanding(player);
		});
}
