#include "BenGameSnapshot.h"

#include "object/WowUnitSnapshot.h"
#include "game/WowGame.h"

BenGameSnapshot::BenGameSnapshot(const WowGame& game) :
	ABenGameSnapshot(game),
	mInGame(nullptr != game.getObjectManager().getActivePlayer()),
	mInCombat(mInGame&& game.getObjectManager().getActivePlayer()->isInCombat())
{
	// Make sure we push every faction to prevent mUnitLists.at(<MissingFaction>) crash
	mUnitLists[Faction::FactionHostile];
	mUnitLists[Faction::FactionNeutral];
	mUnitLists[Faction::FactionFriendly];
	
	const auto self(game.getObjectManager().getActivePlayer());

	if (nullptr != self) {
		const auto allUnits(game.getObjectManager().allOfType<WowUnitObject>());

		for (auto it = allUnits.begin(); it != allUnits.end(); ++it)
		{
			const auto& unit(**it);

			// filter out critters and such

			auto* faction(getUnitFactionList(*self, unit));
			if (nullptr != faction)
			{
				faction->push_back(std::make_unique<WowUnitSnapshot>(**it));
			}
		}
	}
}

BenGameSnapshot::~BenGameSnapshot() = default;

long BenGameSnapshot::getNetworkLatencyMs() const
{
	return 0;
}

bool BenGameSnapshot::isInGame() const
{
	return mInGame;
}

bool BenGameSnapshot::isInCombat() const
{
	return mInCombat;
}

const IBenWowGameSnapshot::UnitList& BenGameSnapshot::getUnitList(const Faction faction) const
{
	return mUnitLists.at(faction);
}

std::shared_ptr<const WowUnitSnapshot> BenGameSnapshot::getUnitByGuid(WowGuid128 guid) const
{
	for (auto itList = mUnitLists.begin(); itList != mUnitLists.end(); ++itList)
	{
		const auto itUnitFound(std::find_if((*itList).second.begin(), (*itList).second.end(), [&guid](const auto& v) { return v->getGuid() == guid; }));

		if (itList->second.end() != itUnitFound)
			return *itUnitFound;
	}

	return nullptr;
}

IBenWowGameSnapshot::UnitList* BenGameSnapshot::getUnitFactionList(const WowActivePlayerObject& player, const WowUnitObject& obj)
{
	if (player.isFriendly(obj))
	{
		return &mUnitLists[Faction::FactionFriendly];
	}
	
	if (player.canAttack(obj))
	{
		return &mUnitLists[Faction::FactionHostile];
	}
	
	return nullptr;
}
