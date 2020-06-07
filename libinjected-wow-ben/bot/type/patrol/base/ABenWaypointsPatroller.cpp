#include "ABenWaypointsPatroller.h"

#include "pathfinder/LinearPathFinder.h"
#include "ServerWowMessage.h"

ABenWaypointsPatroller::ABenWaypointsPatroller(IBenGameplay* gameplay, const std::string& tag, ABenChampion* combatBot) :
	ABenWaypointsPatroller(gameplay, tag, nullptr, combatBot)
{
}

ABenWaypointsPatroller::ABenWaypointsPatroller(IBenGameplay* gameplay, const std::string& tag, ABenTraveler* travelBot, ABenChampion* combatBot) :
	ABenPatroller(gameplay, tag, travelBot, combatBot)
{
}

ABenWaypointsPatroller::~ABenWaypointsPatroller() = default;

bool ABenWaypointsPatroller::handleWowMessage(ServerWowMessage& serverMessage) {
	FileLogger dbg(mDbg, "handleWowMessage");
	bool handled = false;
	dbg << dbg.v() << "handleServerMessage " << (int)serverMessage.type << dbg.endl();

	switch (serverMessage.type)
	{
	case MessageType::POST_DLL_DATA_3DPATH:
		mDbg << mDbg.v() << "Loaded pathfinder with " << serverMessage.waypoints->size() << "waypoints! thanks! " << mDbg.endl();
		mPathFinder.updatePath(*serverMessage.waypoints);
		handled = true;
	default:
		break;
	}

	return handled;
}

IPathFinder& ABenWaypointsPatroller::getPathFinder()
{
	return mPathFinder;
}
