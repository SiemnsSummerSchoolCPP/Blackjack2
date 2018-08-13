#pragma once

#include "SocketConnection/Connection.h"
#include <string>
#include <ostream>

namespace Services
{
	class Logger
	{
	public:
		Logger();
	
		void logAction(std::string msg) const;
		void logAction(
			const SocketConnection::Connection& connection,
			std::string msg) const;
		
	private:
		std::ostream* m_outStream;
		
		void printTime() const;
	};
}
