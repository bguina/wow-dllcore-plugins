#pragma once

#include <set>

#include "../wow/AWowBot.h"
#include "../../process/wow/object/WowGuid128.h"

class WowUnitObject;

class WowMaxBot : public AWowBot
{
public:
	WowMaxBot(WowGame& game);
	virtual ~WowMaxBot();

	virtual void onD3dRender();

	virtual bool handleServerMessage(const PluginServerMessage& serverMessage) override;

protected:
	virtual void _onRunning() override;

	virtual void _logDebug() const override;

	std::unique_ptr<IPathFinder> mPathFinder;
	std::shared_ptr<const WowUnitObject> mTargetUnit;
	std::set<WowGuid128> mBlacklistedGuids;
};

inline std::ostream& operator<<(
	std::ostream& out,
	const class WowMaxBot& obj
	)
{
	out << dynamic_cast<const AWowBot&>(obj);
	return out;
}
