#pragma once

#include <netinet/in.h>
#include <ostream>

namespace SocketConnection
{
	struct Connection
	{
		int socket = -1;
		sockaddr_in address;
	};
	
	std::ostream& operator << (std::ostream& o, const Connection& target);
}
