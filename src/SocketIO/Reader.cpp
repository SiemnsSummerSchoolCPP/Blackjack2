#include "Reader.hpp"
#include "Requests/Request.h"
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
	Requests::RequestLenType msgLen;
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
	
	readValue = ::read(socket, m_buf.data(), msgLen);
	if (readValue == -1)
		return readValue;
	
	m_buf[readValue] = 0;
	
	*result = m_buf.data();
	resultLen = msgLen;
	
	return readValue;
}


int Reader::read(int socket, Requests::Request& requestResult)
{
	int readValue;
	
	readValue = ::read(
		socket,
		&requestResult.dataLen,
		sizeof(requestResult.dataLen));
	
	if (readValue == 0)
		return 0;
	
	if (readValue == -1)
		return -1;
	
	readValue = ::read(
		socket,
		&requestResult.header,
		sizeof(requestResult.header));
	
	if (readValue == 0)
		return 0;
	
	if (readValue == -1)
		return -1;
	
	if (requestResult.dataLen + 1 > m_buf.size())
	{
		m_buf.resize(requestResult.dataLen + 1);
	}
	
	readValue = ::read(socket, m_buf.data(), requestResult.dataLen);
	if (readValue == -1)
		return -1;
	
	m_buf[readValue] = 0;
	requestResult.data = m_buf.data();
	
	return readValue;
}
