#include "pch.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <list>
#include <sstream>

#include "LinearPathFinder.h"
#include "Vector3f.h"

std::list<Vector3f>::const_iterator helperGetNearestWaypoint(const std::list<Vector3f> mPath, const Vector3f& currentPosition);

LinearPathFinder::LinearPathFinder(const std::vector<Vector3f>& waypoints) :
	mPath(waypoints.begin(), waypoints.end()),
	mDestinationWaypoint(mPath.end()),
	mNextWaypoint(mPath.begin())
{
}

LinearPathFinder::~LinearPathFinder() {
}

bool LinearPathFinder::setDestination(const Vector3f& destination) {
	APathFinder::setDestination(destination);
	mDestinationWaypoint = helperGetNearestWaypoint(mPath, *mDestination);
	return mPath.end() != mDestinationWaypoint;
}

void LinearPathFinder::clearDestination() {
	APathFinder::clearDestination();
	mDestinationWaypoint = mPath.end();
}

void LinearPathFinder::reversePath() {
	mPath.reverse();
}

bool LinearPathFinder::findPath(const Vector3f& currentPosition, Vector3f& result) const {
	std::list<Vector3f>::const_iterator nearestWaypoint(helperGetNearestWaypoint(mPath, currentPosition));

	if (nearestWaypoint != mPath.end()) {
		result = *nearestWaypoint;
		return true;
	}

	return false;
}

bool LinearPathFinder::moveAlong(const Vector3f& currentPosition, Vector3f& result) {
	return followPathToDestination(currentPosition, result);
}

bool LinearPathFinder::followPathToDestination(const Vector3f& currentPosition, Vector3f& result) {
	std::stringstream ss;

	if (mNextWaypoint == mPath.end()) {
		mNextWaypoint = helperGetNearestWaypoint(mPath, currentPosition);
	}
	else if (currentPosition.getDistanceTo(*mNextWaypoint) < 10)
	{
		// we have reached the target position, proceed to next waypoint
		// fixme: we should proceed to the path leading to the mDestinationWaypoint!
		mNextWaypoint++;

		// fixme: this should never happen since we are supposed to lead to the mDestinationWaypoint (unless destination is not set?)
		if (mNextWaypoint == mPath.end())
		{
			bool endIsNearStart = currentPosition.getDistanceTo(*mPath.begin()) < 3;

			if (!endIsNearStart)
			{
				// the graph start can't be found, just reverse the list to walk back
				reversePath();
			}

			// otherwise just continue the loop to the graph start
			mNextWaypoint = mPath.begin();
		}
	}
	else {
		// target waypoint is still far
		// target waypoint remains unchanged
	}

	result = *mNextWaypoint;
	return true;
}

size_t LinearPathFinder::getWaypointsCount() const {
	return mPath.size();
}

std::list<Vector3f>::const_iterator helperGetNearestWaypoint(const std::list<Vector3f> mPath, const Vector3f& currentPosition) {
	std::list<Vector3f>::const_iterator nearestWaypoint(mPath.end());
	float nearestDistance = FLT_MAX;

	for (std::list<Vector3f>::const_iterator it = mPath.begin(); it != mPath.end(); it++)
	{
		float waypointDistance = currentPosition.getDistanceTo(*it);

		if (waypointDistance < nearestDistance)
		{
			nearestDistance = waypointDistance;
			nearestWaypoint = it;
		}
	}

	return nearestWaypoint;
}