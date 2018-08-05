#pragma once

#include <vector>
#include "SocketIO.h"

namespace SocketIO
{
	class Writer
	{
	public:
		static Writer& getWriter();
		
		int write(int socket, const void* data, MsgLenType dataLen);
		
	private:
		static Writer instance;
		
		std::vector<unsigned char> m_buf;
	};
}
