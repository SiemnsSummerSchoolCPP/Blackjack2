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
			Requests::RequestLenType dataLen);
		
		int write(int socket, const Requests::Request& request);
		
	private:
		static Writer instance;
		
		std::vector<unsigned char> m_buf;
	};
}
