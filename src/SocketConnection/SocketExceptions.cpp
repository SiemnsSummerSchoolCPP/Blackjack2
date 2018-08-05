#include "Exceptions.h"

using namespace SocketConnection::Exceptions;

SocketExceptionStr::SocketExceptionStr(const std::string msg) : m_msg(msg)
{
}

const char* SocketExceptionStr::what() const throw()
{
	return m_msg.c_str();
}
