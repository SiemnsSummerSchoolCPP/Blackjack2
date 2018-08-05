#pragma once

#include "Connection.h"

namespace SocketConnection
{
	class Client
	{
	public:
		// Getters.
		const Connection& getConnection() const;
		
		void setup(const char* ip, int port);
		
	private:
		Connection m_connection;
	};
}
