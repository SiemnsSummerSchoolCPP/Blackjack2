#include "Request.h"

using namespace Requests;

Request::Request(
	RequestHeaderType header,
	RequestLenType requestLen,
	const unsigned char* data) :
	header(header),
	dataLen(requestLen),
	data(data)
{
}
