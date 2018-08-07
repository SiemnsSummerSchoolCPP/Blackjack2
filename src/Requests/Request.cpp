#include "Request.h"
#include <string>

using namespace Requests;

Request::Request()
{
}

Request::Request(const Request& request) :
	header(request.header),
	dataLen(request.dataLen),
	data(request.data)
{
}

Request::Request(
	const RequestHeader header,
	const RequestLen requestLen,
	const unsigned char* const data) :
	header(header),
	dataLen(requestLen),
	data(data)
{
}

Request::Request(const RequestHeader header, const std::string& str) :
	header(header),
	dataLen(str.length()),
	data(reinterpret_cast<const unsigned char*>(str.c_str()))
{

}
