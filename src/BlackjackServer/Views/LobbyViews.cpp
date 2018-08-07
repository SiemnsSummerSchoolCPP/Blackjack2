#include "Views/LobbyViews.hpp"
#include <sstream>

using namespace BlackjackServer;
using namespace BlackjackServer::Views;

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
