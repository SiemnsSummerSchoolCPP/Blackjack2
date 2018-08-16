#include "AppServices.h"
#include "BlackjackClient/Controllers/MsgReadController.hpp"

using namespace BlackjackServer;

static const Requests::RequestHeader kReceiveMsgHeader =
	BlackjackClient
		::Controllers
		::MsgReadController
		::RequestHeaders
		::kReceiveMsg;

AppServices::AppServices(
	int moneyPrecision,
	const char* moneySymbol,
	NetworkTools::NetworkHost* networkHost,
	DataLayer::BjDatabase* dbContext,
	AppLogic* appLogic) :

	logger(),
	userManager(dbContext),
	sendHelper(networkHost, &logger, kReceiveMsgHeader),
	printHelper(&appLogic->pointsTools, moneyPrecision, moneySymbol)
{
}
