#include "ABenGameSnapshot.h"

#include "game/WowGame.h"
#include "object/WowPlayerSnapshot.h"

ABenGameSnapshot::ABenGameSnapshot(const WowGame& game) :
	mTimestamp(game.getTime()),
	mSelf(nullptr)
{
	const auto self(game.getObjectManager().getActivePlayer());
	if (nullptr != self) mSelf = std::make_unique<WowPlayerObject>(*self);
}

ABenGameSnapshot::~ABenGameSnapshot() = default;

const WowPlayerObject* ABenGameSnapshot::getSelf() const
{
	return mSelf.get();
}

IBenGameSnapshot::Timestamp ABenGameSnapshot::getTimestamp() const
{
	return mTimestamp;
}
