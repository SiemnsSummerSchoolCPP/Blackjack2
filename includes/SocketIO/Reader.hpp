#pragma once

#include "Requests/Request.h"
#include <vector>

namespace SocketIO
{
	class Reader
	{
	public:
		static Reader& getInstance();
		
		int read(int socket, const unsigned char** result, size_t& resultLen);
		int read(int socket, Requests::Request& requestResult);
		
	private:
		static Reader instance;
		
		std::vector<unsigned char> m_buf;
	};
}
