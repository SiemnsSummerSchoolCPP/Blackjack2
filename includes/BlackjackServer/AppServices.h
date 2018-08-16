#pragma once

#include "AppLogic.h"
#include "NetworkTools/NetworkHost.hpp"
#include "DataLayer/BjDatabase.hpp"
#include "Services/Logger.hpp"
#include "Services/UserManager.hpp"
#include "Services/SendHelper.hpp"
#include "Services/PrintHelper.hpp"

namespace BlackjackServer
{
	struct AppServices
	{
		AppServices(
			int moneyPrecision,
			const char* moneySymbol,
			NetworkTools::NetworkHost* networkHost,
			DataLayer::BjDatabase* dbContext,
			AppLogic* appLogic);
	
		Services::Logger logger;
		Services::UserManager userManager;
		Services::SendHelper sendHelper;
		Services::PrintHelper printHelper;
	};
}
