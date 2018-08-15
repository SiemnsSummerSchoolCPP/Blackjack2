#include "UserInputControllers/GameController.hpp"
#include "DataLayer/Bet.h"
#include "BlackjackServer/Controllers/LobbyController.hpp"
#include <regex>

using namespace BlackjackClient;
using namespace BlackjackClient::UserInputControllers;

GameController::GameController(
	const NetworkTools::NetworkClient* networkClient) :
	m_networkClient(networkClient)
{
}

bool GameController::placeABet(std::string userInput) const
{
	static const auto regexPattern = std::regex(
		"^bet (.*)(?:\\$)?$");

	std::smatch regexSearchBuf;
	
	const auto itMatches = std::regex_search(
		userInput,
		regexSearchBuf,
		regexPattern);
	
	if (!itMatches)
		return false;
	
	const auto amountStr = regexSearchBuf[1].str();
	if (amountStr.length() == 0)
	{
		fprintf(stderr, "A value has to be provided.\n");
		return true;
	}
	
	auto bet = DataLayer::Bet();
	try
	{
		bet.amount = std::stod(amountStr);
	}
	catch (std::invalid_argument)
	{
		fprintf(stderr, "'%s': Invalid number.\n", amountStr.c_str());
		return true;
	}
	
	bet.handIndex = 0;
	unsigned char betBuf[sizeof(bet)];
	
	memcpy(betBuf, &bet, sizeof(bet));
	m_networkClient->sendRequest(Requests::Request(
		BlackjackServer::Controllers::GameSessionController::kPlaceBet,
		sizeof(bet),
		betBuf
	));
	
	return true;
}

bool GameController::hit(std::string userInput) const
{
	if (!std::regex_match(userInput, std::regex("^(hit|h)$")))
		return false;
	
	unsigned char betBuf[1];
	
	betBuf[0] = 0;
	m_networkClient->sendRequest(Requests::Request(
		BlackjackServer::Controllers::GameSessionController::kHit,
		sizeof(betBuf),
		betBuf
	));
	
	return true;
}

bool GameController::stand(std::string userInput) const
{
	if (!std::regex_match(userInput, std::regex("^(stand|s)$")))
		return false;
	
	unsigned char betBuf[1];
	
	betBuf[0] = 0;
	m_networkClient->sendRequest(Requests::Request(
		BlackjackServer::Controllers::GameSessionController::kStand,
		sizeof(betBuf),
		betBuf
	));
	
	return true;
}
