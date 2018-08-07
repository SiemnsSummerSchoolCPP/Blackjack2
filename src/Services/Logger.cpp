#include "Logger.hpp"
#include <iostream>
#include <iomanip>

using namespace Services;

Logger::Logger() :
	m_outStream(&std::cout)
{
}

void Logger::logAction(
	const SocketConnection::Connection& connection,
	std::string msg) const
{
	printTime();
	*m_outStream
		<< "[" << connection << "]"
		<< msg
		<< std::endl;
}

/*
** Private helpers.
*/

void Logger::printTime() const
{
	auto now = std::chrono::system_clock::to_time_t(
		std::chrono::system_clock::now());
	
   	*m_outStream << std::put_time(localtime(&now), "[%T]");
}
