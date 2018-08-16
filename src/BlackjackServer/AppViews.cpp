#include "AppViews.h"

using namespace BlackjackServer;

AppViews::AppViews(
	const AppServices* appServices) :
	lobbyViews(&appServices->printHelper),
	gmSessionViews(&appServices->printHelper)
{
}
