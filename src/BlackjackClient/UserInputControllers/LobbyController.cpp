#include "UserInputControllers/LobbyController.hpp"
#include "BlackjackServer/Controllers/LobbyController.hpp"
#include <regex>

using namespace BlackjackClient::UserInputControllers;

LobbyController::LobbyController(
	const NetworkTools::NetworkClient* networkClient) :
	m_networkClient(networkClient)
{
}

bool LobbyController::sendMsg(const std::string userInput) const
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
	
	m_networkClient->sendRequest(Requests::Request(
		BlackjackServer::Controllers::LobbyController::RequestHeaders::kSendMsg,
		msg
	));
	
	return true;
}

bool LobbyController::changeName(const std::string userInput) const
{
	static const auto regexPattern = std::regex(
		"^set name to (.*)$");

	std::smatch regexSearchBuf;
	
	const auto itMatches = std::regex_search(
		userInput,
		regexSearchBuf,
		regexPattern);
	
	if (!itMatches)
		return false;
	
	m_networkClient->sendRequest(Requests::Request(
		BlackjackServer::Controllers::LobbyController::RequestHeaders::kSetName,
		regexSearchBuf[1].str()
	));
	
	return true;
}

bool LobbyController::setReady(const std::string userInput) const
{
	static const auto regexPattern = std::regex(
		"^(ready|set ready)$");

	std::smatch regexSearchBuf;
	
	const auto itMatches = std::regex_search(
		userInput,
		regexSearchBuf,
		regexPattern);
	
	if (!itMatches)
		return false;
	
	const auto header =
		BlackjackServer::Controllers
			::LobbyController
			::RequestHeaders::kSetReady;
	m_networkClient->sendRequest(Requests::Request(header, "dummy str"));
	return true;
}
