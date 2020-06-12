#pragma once

#include "IBenWowGameSnapshot.h"

class ABenGameSnapshot : public IBenWowGameSnapshot {
public:
	ABenGameSnapshot(const WowGame& game);
	virtual ~ABenGameSnapshot();

	const WowPlayerObject* getSelf() const override;
	Timestamp getTimestamp() const override;

protected:
	Timestamp mTimestamp;
	Timestamp mGameTimestamp;
	std::unique_ptr<WowPlayerObject> mSelf;
	std::vector<std::shared_ptr<WowUnitObject>> mHostileUnitsByThreat;
};
