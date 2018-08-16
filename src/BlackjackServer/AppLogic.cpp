#include "AppLogic.h"

using namespace BlackjackServer;

AppLogic::AppLogic(
	DataLayer::BjDatabase* dbContext) :
	pointsTools(),
	playerHandLogic(),
	playerLogic(&playerHandLogic, &pointsTools),
	dealerLogic(&pointsTools, dbContext),
	gameStatusLogic(dbContext, &playerLogic)
{
}
