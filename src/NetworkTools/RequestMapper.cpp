#include "RequestMapper.hpp"
#include "SocketIO/Writer.hpp"

using namespace NetworkTools;

bool RequestMapper::hasAction(Requests::RequestHeader header) const
{
	return m_actions.find(header) != m_actions.end();
}

void RequestMapper::setAction(
	const Requests::RequestHeader header,
	const RequestAction action)
{
	m_actions[header] = action;
}

int RequestMapper::executeAction(
	const Requests::RequestHeader header,
	const SocketConnection::Connection& connection,
	const Requests::Request* request) const
{
	if (!hasAction(header))
		throw NoSuchHeaderException();
	
	return m_actions.at(header)(connection, request);
}


/*
** Exceptions
*/

const char* RequestMapper::NoSuchHeaderException::what() const throw()
{
	return "No such header.";
}
