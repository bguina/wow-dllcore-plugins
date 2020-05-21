#pragma once

#include <cmath>
#include <iostream>
#include <vector>
#include <list>
#include <sstream>

#include "APathFinder.h"

class LinearPathFinder final : public APathFinder
{
public:
	LinearPathFinder(const std::vector<Vector3f>& waypoints);
	~LinearPathFinder();

	virtual bool setDestination(const Vector3f& destination) override;

	virtual void clearDestination() override;

	virtual bool findPath(const Vector3f& currentPosition, Vector3f& result) const;

	virtual bool moveAlong(const Vector3f& currentPosition, Vector3f& result);

	virtual bool followPathToDestination(const Vector3f& currentPosition, Vector3f& result);

	void reversePath();

protected:
	std::list<Vector3f> mPath;
	mutable std::list<Vector3f>::const_iterator mNextWaypoint;
	std::list<Vector3f>::const_iterator mDestinationWaypoint;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class LinearPathFinder& obj
	)
{
	out << "[LinearPathFinder]";
	// TODO print navigator state
	return out;
}
