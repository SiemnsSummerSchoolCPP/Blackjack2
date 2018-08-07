#pragma once

#include "Requests/Request.h"
#include <vector>

namespace SocketIO
{
	class Writer
	{
	public:
		static Writer& getWriter();
		
		int write(
			int socket,
			const void* data,
			Requests::RequestLen dataLen);
		
		int write(int socket, const Requests::Request& request);
		int broadcast(
			const std::vector<int>& sockets,
			const Requests::Request& request,
			bool ignoreFailures = true);
		
	private:
		static Writer instance;
		
		std::vector<unsigned char> m_buf;
	};
}
