#pragma once

#include "../WowBot.h"

#include "../../../../pathfinder/IPathWaypointsConsumer.h"

class IPathFinder;

class BenBot : public WowBot, public IPathWaypointsConsumer
{
public:
	BenBot(WowGame& game, const std::string& tag);
	virtual ~BenBot();

	virtual void pause(bool paused) override;
	virtual void run() override;

	virtual void loadPathWaypoints(const std::vector<Vector3f>& waypoints) override;

protected:
	void logDebug() const;

	std::unique_ptr<IPathFinder> mPathFinder;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class BenBot& obj
	)
{
	out << dynamic_cast<const WowBot&>(obj);
	return out;
}
