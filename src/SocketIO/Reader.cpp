#include "Reader.hpp"
#include "SocketIO.h"
#include <unistd.h>

using namespace SocketIO;

Reader& Reader::getInstance()
{
	return instance;
}

int Reader::read(
	const int socket,
	const unsigned char** const result,
	size_t& resultLen)
{
	MsgLenType msgLen;
	int readValue;
	
	readValue = ::read(socket, &msgLen, sizeof(msgLen));
	if (readValue == 0)
		return 0;
	
	if (readValue == -1)
		return -1;
	
	if (msgLen + 1 > m_buf.size())
	{
		m_buf.resize(msgLen + 1);
	}
	
	readValue = ::read(socket, &msgLen, sizeof(msgLen));
	m_buf[readValue] = 0;
	
	*result = m_buf.data();
	resultLen = msgLen;
	
	return readValue;
}
