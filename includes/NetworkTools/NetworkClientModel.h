#pragma once

#include "RequestMapper.hpp"

namespace NetworkTools
{
	struct NetworkClientModel
	{
		int port = 0;
		const char* ip;
		const RequestMapper* requestMapper;
	};
}
