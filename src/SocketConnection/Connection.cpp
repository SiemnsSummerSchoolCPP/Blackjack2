#include "Connection.h"
#include <arpa/inet.h>

using namespace SocketConnection;

std::ostream& SocketConnection::operator << (
	std::ostream& stream,
	const Connection& target)
{
	stream
		<< inet_ntoa(target.address.sin_addr) << ":"
		<< ntohs(target.address.sin_port);
	return stream;
}
