#pragma once

#include "IBenGameSnapshot.h"

class ABenGameSnapshot : public IBenGameSnapshot {
public:
	ABenGameSnapshot(const WowGame& game);
	virtual ~ABenGameSnapshot();

	const WowPlayerObject* getSelf() const override;
	Timestamp getTimestamp() const override;

protected:
	Timestamp mTimestamp;
	std::unique_ptr<WowPlayerObject> mSelf;
	std::vector<std::shared_ptr<WowUnitObject>> mHostileUnitsByThreat;
};
