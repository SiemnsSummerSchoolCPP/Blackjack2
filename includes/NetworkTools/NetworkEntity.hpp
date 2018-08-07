#pragma once

#include "RequestMapper.hpp"

namespace NetworkTools
{
	class NetworkEntity
	{
	public:
		enum RequestHeaders : Requests::RequestHeader
		{
			kNewConnectionHeader = 1,
			kEndConnectionHeader = 2
		};
	
		NetworkEntity(const RequestMapper& requestMapper);

	protected:
		const RequestMapper& m_requestMapper;
		
		int onNewConnection(
			const SocketConnection::Connection& connection) const;
		
		int onEndConnection(
			const SocketConnection::Connection& connection) const;
		
		int onConnectionMsg(
			const SocketConnection::Connection& connection,
			const Requests::Request& request) const;
	};
}
