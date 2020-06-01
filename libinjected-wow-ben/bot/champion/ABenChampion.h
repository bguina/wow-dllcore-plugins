#pragma once

#include <memory>
#include <string>
#include <set>

#include "../base/ABen.h"

class ABenChampion : public ABen {
public:
	ABenChampion(const std::string& tag, ABenChampion* renegate);
	virtual ~ABenChampion();

	virtual bool runAway();

	virtual bool isPositionSatisfying(const WowVector3f& position) = 0;
	virtual bool concernAround(std::list<WowGuid128>& result) = 0;

protected:
	std::unique_ptr<ABenChampion> mRenegate;
	std::list<WowUnitObject> mKillList;
};

