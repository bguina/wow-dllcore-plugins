#include "WowUnitSnapshot.h"
#include "game/object/WowUnitObject.h"

WowUnitSnapshot::WowUnitSnapshot(const WowUnitObject& obj) :
	WowObjectSnapshot(obj),
	mRace(obj.getRace()),
	mClass(obj.getClass()),
	mLevel(obj.getLevel()),
	mHealth(obj.getHealth()),
	mMaxHealth(obj.getMaxHealth()),
	mEnergy(obj.getEnergy()),
	mMaxEnergy(obj.getMaxEnergy()),
	mIsInCombat(obj.isInCombat()),
	mLootable(obj.isLootable()),
	mTappedByOther(obj.isTappedByOther()),
	mTappedByMe(obj.isTappedByMe()),
	mSummonedBy(obj.getSummonedBy().upper(), obj.getSummonedBy().lower()),
	mTarget(obj.getTargetGuid().upper(), obj.getTargetGuid().lower())
{
}

WowUnitSnapshot::~WowUnitSnapshot() = default;

WowUnitClass WowUnitSnapshot::getClass() const
{
	return mClass;
}

WowUnitRace WowUnitSnapshot::getRace() const
{
	return mRace;
}

int WowUnitSnapshot::getLevel() const
{
	return mLevel;
}

int WowUnitSnapshot::getHealth() const
{
	return mHealth;
}

int WowUnitSnapshot::getMaxHealth() const
{
	return mMaxHealth;
}

bool WowUnitSnapshot::isDead() const
{
	return 1 > mHealth;
}

int WowUnitSnapshot::getEnergy() const
{
	return mEnergy;
}

int WowUnitSnapshot::getMaxEnergy() const
{
	return mMaxEnergy;
}

bool WowUnitSnapshot::isInCombat() const
{
	return mIsInCombat;
}

bool WowUnitSnapshot::isLootable() const
{
	return mLootable;
}

bool WowUnitSnapshot::isTappedByOther() const
{
	return mTappedByOther;
}

bool WowUnitSnapshot::isTappedByMe() const
{
	return mTappedByMe;
}

WowGuid128 WowUnitSnapshot::getSummonedBy() const
{
	return mSummonedBy;
}

WowGuid128 WowUnitSnapshot::getTargetGuid() const
{
	return mTarget;
}


