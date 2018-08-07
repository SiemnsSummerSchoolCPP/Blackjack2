#pragma once

#include "RequestMapper.hpp"

namespace NetworkTools
{
	struct NetworkHostModel
	{
		int port = 0;
		size_t maxConnections = 5;
		size_t maxPendings = 3;
		const RequestMapper* requestMapper;
	};
}
