#pragma once

#include "NetworkTools/NetworkClient.hpp"
#include <string>

namespace BlackjackClient { namespace UserInputControllers
{
	class GameController
	{
	public:
		GameController(const NetworkTools::NetworkClient* networkClient);
		
		bool placeABet(std::string userInput) const;
		bool hit(std::string userInput) const;
		bool stand(std::string userInput) const;
		
	private:
		const NetworkTools::NetworkClient* m_networkClient;
	};
}}
