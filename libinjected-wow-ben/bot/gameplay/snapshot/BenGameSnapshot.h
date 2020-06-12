#pragma once

#include <map>

#include "ABenGameSnapshot.h"

class BenGameSnapshot final : public ABenGameSnapshot  {
public:

	
	BenGameSnapshot(const WowGame& game);
	virtual ~BenGameSnapshot();

	long getNetworkLatencyMs() const override;
	bool isInGame() const override;
	bool isInCombat() const override;

	const UnitList& getUnitList(Faction faction) const override;

	std::shared_ptr<const WowUnitSnapshot> getUnitByGuid(WowGuid128 guid) const override;

protected:
	UnitList* getUnitFactionList(const WowActivePlayerObject& player, const WowUnitObject& obj);

	bool mInGame;
	bool mInCombat;

	std::map<Faction, UnitList> mUnitLists;
};
