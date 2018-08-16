#include "AppControllers.h"

using namespace BlackjackServer;

AppControllers::AppControllers(
	DataLayer::BjDatabase* dbContext,
	AppLogic* appLogic,
	AppServices* appServices,
	AppViews* appViews) :

	lobbyCtrl(
		&appViews->lobbyViews,
		
		&appServices->logger,
		&appServices->sendHelper,
		&appServices->userManager,

		dbContext,
		&gmSessionCtrl),

	gmSessionCtrl(
		&appViews->gmSessionViews,
		
		&appServices->logger,
		&appServices->sendHelper,
		&appServices->userManager,
		
		dbContext,
		
		&appLogic->gameStatusLogic,
		&appLogic->playerLogic,
		&appLogic->dealerLogic)
{
}
