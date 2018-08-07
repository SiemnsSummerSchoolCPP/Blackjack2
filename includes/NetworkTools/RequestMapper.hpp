#pragma once

#include "Requests/Request.h"
#include "SocketConnection/Connection.h"
#include <map>
#include <functional>

namespace NetworkTools
{
	class RequestMapper
	{
	public:
		typedef std::function<int (
			const SocketConnection::Connection&,
			const Requests::Request*)> RequestAction;

		struct NoSuchHeaderException;
		
		bool hasAction(Requests::RequestHeader header) const;
		void setAction(
			Requests::RequestHeader header,
			RequestAction action);
		
		int executeAction(
			Requests::RequestHeader header,
			const SocketConnection::Connection& connection,
			const Requests::Request* request) const;
		
	private:
		std::map<Requests::RequestHeader, RequestAction> m_actions;
	};
	
	struct RequestMapper::NoSuchHeaderException : std::exception
	{
		const char* what() const throw();
	};
}
