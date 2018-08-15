#pragma once

#include "BlackjackServer/Models/LobbyModels.h"
#include "DataLayer/UserModel.h"
#include "Services/PrintHelper.hpp"
#include <string>

namespace BlackjackServer { namespace Views
{
	class LobbyViews
	{
	public:
		LobbyViews(const Services::PrintHelper* printHelper);
	
		std::string clientJoin_View(
			const Models::LobbyModels::ClientJoinModel& model) const;
		std::string clientClientLeave_View(
			const Models::LobbyModels::ClientLeaveModel& model) const;
		std::string allPlayersLeft_View() const;
		
		std::string sendMsg_View(
			const Models::LobbyModels::SendMsgModel& model) const;
		
		std::string changeName_View(
			const Models::LobbyModels::ChangeNameModel& model) const;
		std::string invalidName_WhisperView(
			const std::string& name, const char* msg) const;
		std::string invalidName_ServerView(
			const Models::LobbyModels::InvalidChangeNameModel_Server& m) const;
		
		std::string anotherGameIsBeingPlayed_View() const;
		std::string setReadyAnotherGameIsBeingPlayed_ServerView() const;
		std::string userIsUnableToPlay_View(const DataLayer::UserModel&) const;
		
		std::string alreadyReady_WhisperView() const;
		std::string waitingForOthers_View(
			const std::vector<std::string>& names) const;
		std::string alreadyReady_ServerView(std::string name) const;
		std::string successSetReady_View(std::string name) const;
	
	private:
		const Services::PrintHelper* m_printHelper;
	};
}}
