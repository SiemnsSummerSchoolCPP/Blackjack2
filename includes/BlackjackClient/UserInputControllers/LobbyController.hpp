#pragma once

#include "NetworkTools/NetworkClient.hpp"
#include <string>

namespace BlackjackClient { namespace UserInputControllers
{
	class LobbyController
	{
	public:
		LobbyController(const NetworkTools::NetworkClient& networkClient);
	
		bool sendMsg(std::string userInput) const;
		bool changeName(std::string userInput) const;
		bool setReady(std::string userInput) const;
	
	private:
		const NetworkTools::NetworkClient& m_networkClient;
	};
}}
