#pragma once

#include <netinet/in.h>

namespace SocketConnection
{
	struct Connection
	{
		int socket = -1;
		sockaddr address;
	};
}
