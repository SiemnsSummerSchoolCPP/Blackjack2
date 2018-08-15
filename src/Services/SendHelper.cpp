#include "SendHelper.hpp"
#include "SocketIO/Writer.hpp"
#include <iomanip>
#include <sstream>

using namespace Services;

SendHelper::SendHelper(
	const NetworkTools::NetworkHost& networkHost,
	Services::Logger& logger,
	const Requests::RequestHeader msgHeader) :
	m_networkHost(networkHost),
	m_logger(logger),
	m_msgHeader(msgHeader)
{
}

void SendHelper::whisperMsg(const int socket, const std::string& msg) const
{
	const auto finalMsg = appendCandyStuff(msg);
	const auto resultRequest = Requests::Request(m_msgHeader, finalMsg);
	SocketIO::Writer::getWriter().write(socket, resultRequest);
}

void SendHelper::whisperMsg(
	const SocketConnection::Connection& connection,
	const std::string& msg) const
{
	whisperMsg(connection.socket, msg);
}

void SendHelper::broadcastMsg(
	const std::string& msg,
	const SocketConnection::Connection* const exceptConnection) const
{
	const auto finalMsg = appendCandyStuff(msg);
	const auto resultRequest = Requests::Request(m_msgHeader, finalMsg);
	m_networkHost.broadcastRequest(resultRequest, exceptConnection);
}

void SendHelper::logAndBroadcast(const std::string& msg) const
{
	m_logger.logAction(msg);
	broadcastMsg(msg);
}

std::string SendHelper::appendCandyStuff(const std::string& msg) const
{
	return makeMsgPrefix() + msg;
}

std::string SendHelper::makeMsgPrefix() const
{
	auto now =
		std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	
	auto ss = std::stringstream();
	ss << std::put_time(localtime(&now), "[%T] ");
	
	return ss.str();
}
