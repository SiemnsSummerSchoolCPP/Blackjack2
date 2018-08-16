#include "UserManager.hpp"
#include <algorithm>

using namespace Services;

UserManager::UserManager(
	const DataLayer::BjDatabase* dbContext) :
	m_dbContext(dbContext)
{
}

bool UserManager::userIsBankrupt(const DataLayer::UserModel& user) const
{
	return user.money <= std::numeric_limits<double>::epsilon();
}

bool UserManager::userCanJoin(const DataLayer::UserModel& user) const
{
	return !userIsBankrupt(user);
}

bool UserManager::gameIsReady() const
{
	static const auto notStarted = DataLayer::GameSession::State::kNotStarted;
	
	if (m_dbContext->gameSession.state != notStarted)
		return false;
	
	if (getJoinables().size() == 0)
		return false;
	
	if (getUsersToWait().size() == 0)
		return true;
	
	return false;
}

bool UserManager::userIsAPlayer(const DataLayer::UserModel& user) const
{
	for (const auto& pair : m_dbContext->players)
	{
		if (pair.first == user.uniqueId)
		{
			return true;
		}
	}
	
	return false;
}

void UserManager::unreadyAllPlayers() const
{
	for (auto& pair : m_dbContext->players)
	{
		pair.second->userModel->joinState.isReady = false;
	}
}

std::vector<DataLayer::PlayerModel*> UserManager::getPlayers() const
{
	std::vector<DataLayer::PlayerModel*> result;
	auto& playersMap = m_dbContext->players;
	
	result.resize(playersMap.size());
	std::transform(
		playersMap.begin(),
		playersMap.end(),
		result.begin(),
		[](const auto& pair)
		{
			return pair.second.get();
		});
	
	return result;
}

std::vector<DataLayer::UserModel*> UserManager::getJoinables() const
{
	std::vector<DataLayer::UserModel*> result;
	
	for (auto& pair : m_dbContext->users)
	{
		if (userCanJoin(*pair.second))
		{
			result.push_back(pair.second.get());
		}
	}
	
	return result;
}

std::vector<DataLayer::UserModel*> UserManager::getUsersToWait() const
{
	std::vector<DataLayer::UserModel*> result;
	for (auto& user : getJoinables())
	{
		if (!user->joinState.isReady)
		{
			result.push_back(user);
		}
	}

	return result;
}
