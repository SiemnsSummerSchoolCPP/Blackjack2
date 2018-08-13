#pragma once

#include "SocketConnection/Connection.h"
#include <string>
#include <vector>

namespace BlackjackServer { namespace Models { namespace LobbyModels
{
	struct SendMsgModel
	{
		std::string name;
		std::string msg;
		SocketConnection::Connection* connection;
	};
	
	struct ClientLeaveModel
	{
		std::string name;
	};
	
	struct ClientJoinModel
	{
		std::string name;
	};
	
	struct ChangeNameModel
	{
		std::string oldName;
		std::string newName;
	};
	
	struct InvalidChangeNameModel_Server
	{
		std::string oldName;
		std::string newName;
		const char* errorMsg;
	};
}}}
