#pragma once

#include "AppLogic.h"
#include "AppServices.h"
#include "AppViews.h"

#include "Controllers/LobbyController.hpp"
#include "Controllers/GameSessionController.hpp"
#include <memory>

namespace BlackjackServer
{
	struct AppControllers
	{
		AppControllers(
			DataLayer::BjDatabase* dbContext,
			AppLogic* appLogic,
			AppServices* appServices,
			AppViews* appViews);
		
		Controllers::LobbyController lobbyCtrl;
		Controllers::GameSessionController gmSessionCtrl;
	};
}
