#include "UserInputControllers/LobbyController.hpp"
#include "BlackjackServer/Controllers/LobbyController.hpp"
#include <regex>

using namespace BlackjackClient::UserInputControllers;

LobbyController::LobbyController(
	const NetworkTools::NetworkClient& networkClient) :
	m_networkClient(networkClient)
{
}

bool LobbyController::sendMsg(std::string userInput) const
{
	static const auto regexPattern = std::regex(
		"^send msg: (.*)$");

	std::smatch regexSearchBuf;
	
	const auto itMatches = std::regex_search(
		userInput,
		regexSearchBuf,
		regexPattern);
	
	if (!itMatches)
		return false;
	
	const auto msg = regexSearchBuf[1].str();
	if (msg.length() == 0)
	{
		fprintf(stderr, "'%s': Invalid message.\n", msg.c_str());
		return true;
	}
	
	m_networkClient.sendRequest(Requests::Request(
		BlackjackServer::LobbyController::RequestHeaders::kSendMsg,
		msg
	));
	
	return true;
}
