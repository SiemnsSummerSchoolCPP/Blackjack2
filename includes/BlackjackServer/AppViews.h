#pragma once

#include "AppServices.h"
#include "Views/LobbyViews.hpp"
#include "Views/GameSessionViews.hpp"

namespace BlackjackServer
{
	struct AppViews
	{
		AppViews(const AppServices* appServices);
		
		Views::LobbyViews lobbyViews;
		Views::GameSessionViews gmSessionViews;
	};
}
