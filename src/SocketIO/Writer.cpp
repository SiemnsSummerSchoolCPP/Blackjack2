#include "Writer.hpp"
#include "SocketIO.h"
#include <unistd.h>
#include <memory>

using namespace SocketIO;

Writer& Writer::getWriter()
{
	return instance;
}

/*
** The buf is used to do I single write. Otherwise, I'd need to write
** the msg len first, then the msg.
*/

int Writer::write(
	const int socket,
	const void* const data,
	const MsgLenType dataLen)
{
	const auto requiredBufLen = dataLen + sizeof(dataLen);
	if (m_buf.size() < requiredBufLen)
	{
		m_buf.resize(requiredBufLen);
	}
	
	memcpy(m_buf.data(), &dataLen, sizeof(dataLen));
	memcpy(m_buf.data() + sizeof(dataLen), data, dataLen);
	
	return ::write(socket, m_buf.data(), dataLen + sizeof(dataLen));
}
