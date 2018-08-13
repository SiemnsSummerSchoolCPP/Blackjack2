#pragma once

#include <cstdint>
#include <string>

namespace Requests
{
	using RequestHeader = uint32_t;
	using RequestLen = uint32_t;
	
	struct Request
	{
		RequestHeader header;
		RequestLen dataLen;
		const unsigned char* data;
		
		Request();
		Request(const Request& request);
		Request(RequestHeader header, const std::string& str);
		Request(
			RequestHeader header,
			RequestLen requestLen,
			const unsigned char* data);
		
		std::string str() const;
	};
}
