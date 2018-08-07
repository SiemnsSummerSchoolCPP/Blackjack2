#pragma once

#include "SocketConnection/Connection.h"
#include "Requests/Request.h"

namespace BlackjackClient
{
	class MsgReadController
	{
	public:
		enum RequestHeaders : Requests::RequestHeader
		{
			kReceiveMsg = 100 + 0
		};
	
		int receiveMsg(
			const SocketConnection::Connection& connection,
			const Requests::Request& request) const;
		
	private:
	};
}
