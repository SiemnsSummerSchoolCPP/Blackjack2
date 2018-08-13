#include "Writer.hpp"
#include <unistd.h>
#include <memory>

using namespace SocketIO;

Writer Writer::instance = Writer();

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
	const Requests::RequestLen dataLen)
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

int Writer::write(const int socket, const Requests::Request& request)
{
	const auto requiredBufLen =
		request.dataLen +
		sizeof(request.dataLen) +
		sizeof(request.header) + 1;
	
	if (m_buf.size() < requiredBufLen)
	{
		m_buf.resize(requiredBufLen);
	}
	
	memcpy(
		m_buf.data(),
		&request.dataLen,
		sizeof(request.dataLen));
	
	memcpy(
		m_buf.data() + sizeof(request.dataLen),
		&request.header,
		sizeof(request.header));
	
	memcpy(
		m_buf.data() + sizeof(request.dataLen) + sizeof(request.header),
		request.data,
		request.dataLen);
	
	const auto totalMsgLen =
		sizeof(request.dataLen) +
		sizeof(request.header) +
		request.dataLen;
	
	return ::write(socket, m_buf.data(), totalMsgLen);
}

int Writer::broadcast(
	const std::vector<int>& sockets,
	const Requests::Request& request,
	const bool ignoreFailures)
{
	int finalResult = 0;
	
	for (const auto& socket : sockets)
	{
		const auto writeResult = getWriter().write(socket, request);
		finalResult = std::min(finalResult, writeResult);
		if (writeResult <= 0 && !ignoreFailures)
		{
			return finalResult;
		}
	}
	return finalResult;
}
