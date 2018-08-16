#pragma once

#include "Controllers/LobbyController.hpp"
#include "Controllers/GameSessionController.hpp"
#include <memory>

namespace BlackjackServer
{
	struct AppControllers
	{
		std::unique_ptr<Controllers::LobbyController> lobbyCtrl;
		std::unique_ptr<Controllers::GameSessionController> gmSessionCtrl;
	};
}
