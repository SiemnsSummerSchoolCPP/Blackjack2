#pragma once

#include <cstdint>

namespace Requests
{
	using RequestHeaderType = uint32_t;
	using RequestLenType = uint32_t;
	
	struct Request
	{
		RequestHeaderType header;
		RequestLenType dataLen;
		const unsigned char* data;
		
		Request(
			RequestHeaderType header,
			RequestLenType requestLen,
			const unsigned char* data);
	};
}
