#pragma once

struct Vector3f;

class IPathFinder
{
public:
	virtual ~IPathFinder() {}

	// Get the previously set desired destination given to the pathfinder
	// return false if no destination is currently set
	virtual bool getDestination(Vector3f destination) const = 0;

	// Tell the pathfinder our desired final position
	// return false if no path could be found (unreachable position or invalid path)
	virtual	bool setDestination(const Vector3f& destination) = 0;

	// Just tell the pathfinder to shut up
	virtual void clearDestination() = 0;

	// Get the nearest waypoint to the given current position.
	// return false if no path could be found (unreachable position or invalid path)
	virtual bool findPath(const Vector3f& currentPosition, Vector3f& result) const = 0;

	// Get the next waypoint recommended to given destination.
	// If no destination is set, get an appropriate waypoint to "move along" the path.
	// return false if no appropriate waypoint could be found (unreachable position or invalid path)
	virtual bool moveAlong(const Vector3f& currentPosition, Vector3f& result) = 0;

	// Get a guaranteed approximation of the destination the pathfinder will lead to.
	// return false if no guaranteed waypoint could be found (no destination, unreachable position or invalid path)
	virtual bool getGuaranteedDestination(Vector3f& result) const = 0;

	// Find the next waypoint leading to destination.
	// return false if no path could be found (no destination, unreachable position or invalid path)
	virtual	bool followPathToDestination(const Vector3f& currentPosition, Vector3f& result) = 0;

};
