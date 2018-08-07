#include "NetworkEntity.hpp"

using namespace NetworkTools;

NetworkEntity::NetworkEntity(
	const RequestMapper& requestMapper) :
	m_requestMapper(requestMapper)
{
}

int NetworkEntity::onNewConnection(
	const SocketConnection::Connection& connection) const
{
	if (!m_requestMapper.hasAction(kNewConnectionHeader))
		return 0;

	return m_requestMapper.executeAction(
		kNewConnectionHeader,
		connection,
		nullptr);
}

int NetworkEntity::onEndConnection(
	const SocketConnection::Connection& connection) const
{
	if (!m_requestMapper.hasAction(RequestHeaders::kEndConnectionHeader))
		return 0;

	return m_requestMapper.executeAction(
		kEndConnectionHeader,
		connection,
		nullptr);
}

int NetworkEntity::onConnectionMsg(
	const SocketConnection::Connection& connection,
	const Requests::Request& request) const
{
	return m_requestMapper.executeAction(
		request.header,
		connection,
		&request);
}
