#pragma once

#include <set>

#include "../bot/BaseWowBot.h"
#include "../../injectable/wow/game/object/WowGuid128.h"
#include "../../injectable/wow/game/object/WowActivePlayerObject.h"

class IPathFinder;
class WowUnitObject;

class WowMaxBot : public BaseWowBot
{
public:
	WowMaxBot(WowGame& game);
	virtual ~WowMaxBot();

	virtual void onResume() override;
	virtual void onPause() override;
	virtual void onEvaluate() override;

	virtual bool handleServerMessage(ClientMessage& serverMessage) override;

protected:

	virtual void _logDebug() const override;

	std::unique_ptr<IPathFinder> mPathFinder;
	std::shared_ptr<const WowUnitObject> mTargetUnit;
	std::set<WowGuid128> mBlacklistedGuids;
	bool mOpeningCombat = true;
	bool mInteractWith = true;
	bool cacAttack = true;
	int toLoop = 0;
	uint64_t mLastFeedPet;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WowMaxBot& obj
	)
{
	out << dynamic_cast<const BaseWowBot&>(obj);
	return out;
}
