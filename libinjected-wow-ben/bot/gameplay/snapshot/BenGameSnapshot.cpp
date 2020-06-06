#include "BenGameSnapshot.h"

#include "object/WowUnitSnapshot.h"
#include "game/WowGame.h"

BenGameSnapshot::BenGameSnapshot(const WowGame& game) :
	ABenGameSnapshot(game)
{
	const auto self(game.getObjectManager().getActivePlayer());

	if (nullptr != self) {
		const auto allUnits(game.getObjectManager().allOfType<WowUnitObject>());

		for (auto it = allUnits.begin(); it != allUnits.end(); ++it)
		{
			const auto& unit(**it);
			auto& factionList(self->canAttack(game, unit) ? mHostileUnits : mNonHostileUnits);

			factionList.push_back(std::make_unique<WowUnitSnapshot>(**it));
		}
	}
}

BenGameSnapshot::~BenGameSnapshot() = default;

long BenGameSnapshot::getNetworkLatencyMs() const
{
	return 0;
}

const IBenGameSnapshot::UnitList& BenGameSnapshot::listHostiles() const
{
	return mHostileUnits;
}

const IBenGameSnapshot::UnitList& BenGameSnapshot::listNonHostiles() const
{
	return mNonHostileUnits;
}
