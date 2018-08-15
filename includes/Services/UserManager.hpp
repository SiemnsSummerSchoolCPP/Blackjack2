#pragma once

#include "DataLayer/BjDatabase.hpp"

namespace Services
{
	class UserManager
	{
	public:
		UserManager(const DataLayer::BjDatabase* dbContext);
		
		bool userIsBankrupt(const DataLayer::UserModel& user) const;
		bool userCanJoin(const DataLayer::UserModel& user) const;
		bool gameIsReady() const;
		bool userIsAPlayer(const DataLayer::UserModel& user) const;
		
		void unreadyAllPlayers() const;
		
		std::vector<DataLayer::PlayerModel*> getPlayers() const;
		std::vector<DataLayer::UserModel*> getJoinables() const;
		std::vector<DataLayer::UserModel*> getUsersToWait() const;
		
	private:
		const DataLayer::BjDatabase* m_dbContext;
	};
}
