#include "BenGameSnapshot.h"

#include "object/WowUnitSnapshot.h"
#include "game/WowGame.h"

BenGameSnapshot::BenGameSnapshot(const WowGame& game) :
	ABenGameSnapshot(game),
	mInGame(nullptr != game.getObjectManager().getActivePlayer()),
	mInCombat(mInGame&& game.getObjectManager().getActivePlayer()->isInCombat())
{
	const auto self(game.getObjectManager().getActivePlayer());

	if (nullptr != self) {
		const auto allUnits(game.getObjectManager().allOfType<WowUnitObject>());

		for (auto it = allUnits.begin(); it != allUnits.end(); ++it)
		{
			const auto& unit(**it);
			auto& factionList(/*self->isFriendly(game, unit) ? mNonHostileUnits :*/ mHostileUnits);

			factionList.push_back(std::make_unique<WowUnitSnapshot>(**it));
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

const IBenGameSnapshot::UnitList& BenGameSnapshot::getHostileList() const
{
	return mHostileUnits;
}

const IBenGameSnapshot::UnitList& BenGameSnapshot::getNonHostileList() const
{
	return mNonHostileUnits;
}

std::shared_ptr<const WowUnitSnapshot> BenGameSnapshot::getUnitByGuid(WowGuid128 guid) const
{
	const auto itHostile(std::find_if(mHostileUnits.begin(), mHostileUnits.end(), [&guid](const auto& v) { return v->getGuid() == guid; }));

	if (mHostileUnits.end() == itHostile)
	{
		const auto itFriend(std::find_if(mNonHostileUnits.begin(), mNonHostileUnits.end(), [&guid](const auto& v) { return v->getGuid() == guid; }));

		if (mNonHostileUnits.end() == itFriend)
		{
			return nullptr;
		}
		return *itFriend;
	}
	return *itHostile;
}
