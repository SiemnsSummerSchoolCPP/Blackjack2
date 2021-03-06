#include "Views/LobbyViews.hpp"
#include <sstream>

using namespace BlackjackServer;
using namespace BlackjackServer::Views;

LobbyViews::LobbyViews(
	const Services::PrintHelper* printHelper) :
	m_printHelper(printHelper)
{
}

std::string LobbyViews::clientJoin_View(
	const Models::LobbyModels::ClientJoinModel& model) const
{
	auto ss = std::stringstream();
	
	ss << "User " << model.name << " has joined.";
	return ss.str();
}

std::string LobbyViews::clientClientLeave_View(
	const Models::LobbyModels::ClientLeaveModel& model) const
{
	auto ss = std::stringstream();
	
	ss << "User " << model.name << " has left.";
	return ss.str();
}

std::string LobbyViews::allPlayersLeft_View() const
{
	return "All players have left the game.";
}

std::string LobbyViews::sendMsg_View(
	const Models::LobbyModels::SendMsgModel& model) const
{
	auto ss = std::stringstream();
	
	ss << "[" << model.name << "]: " << model.msg;
	return ss.str();
}

/*
** Change name.
*/

std::string LobbyViews::changeName_View(
	const Models::LobbyModels::ChangeNameModel& model) const
{
	auto ss = std::stringstream();
	
	ss << model.oldName << " changed his name to " << model.newName;
	return ss.str();
}

std::string LobbyViews::invalidName_WhisperView(
	const std::string& name,
	const char* msg) const
{
	auto ss = std::stringstream();
	
	ss << name << ": invalid name: " << msg;
	return ss.str();
}

std::string LobbyViews::invalidName_ServerView(
	const Models::LobbyModels::InvalidChangeNameModel_Server& model) const
{
	auto ss = std::stringstream();
	
	ss
		<< "[Failed change name attempt] "
		<< "From " << model.oldName << " to " << model.newName << ": "
		<< model.errorMsg;
	return ss.str();
}

/*
** Set ready.
*/

std::string LobbyViews::anotherGameIsBeingPlayed_View() const
{
	return "Another game is being played at the moment.";
}

std::string LobbyViews::setReadyAnotherGameIsBeingPlayed_ServerView() const
{
	std::stringstream ss;
	
	ss << "Failed to set ready: " << anotherGameIsBeingPlayed_View();
	return ss.str();
}

std::string LobbyViews::userIsUnableToPlay_View(
	const DataLayer::UserModel& user) const
{
	std::stringstream ss;
	
	ss	<< "You are unable to join. Current balance: "
		<< m_printHelper->formatMoney(user.money)
		<< ".";
	return ss.str();
}

std::string LobbyViews::alreadyReady_WhisperView() const
{
	return "You are already set to ready.";
}

std::string LobbyViews::waitingForOthers_View(
	const std::vector<std::string>& names) const
{
	auto ss = std::stringstream();
	
	ss << "Waiting for: " << std::endl;
	for (const auto& name : names)
	{
		ss << "- " << name << std::endl;
	}
	return ss.str();
}

std::string LobbyViews::alreadyReady_ServerView(
	const std::string name) const
{
	auto ss = std::stringstream();
	
	ss << "[Repeated Ready Set]" << name << " tried to set ready again.";
	return ss.str();
}

std::string LobbyViews::successSetReady_View(std::string name) const
{
	auto ss = std::stringstream();
	
	ss << name << " is now ready.";
	return ss.str();
}

