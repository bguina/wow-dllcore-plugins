#pragma once

#include "WowObjectSnapshot.h"
#include "game/object/WowUnitObject.h"

class WowUnitSnapshot : public WowObjectSnapshot
{
public:
	
	WowUnitSnapshot(const WowUnitObject& obj);
	virtual ~WowUnitSnapshot();

	WowUnitClass getClass() const ;
	WowUnitRace getRace() const ;
	int getLevel() const ;
	int getHealth() const ;
	int getMaxHealth() const ;
	bool isDead() const;
	int getEnergy() const ;
	int getMaxEnergy() const ;
	bool isInCombat() const ;
	bool isLootable() const ;
	bool isTappedByOther() const ;
	bool isTappedByMe() const ;
	WowGuid128 getSummonedBy() const ;
	WowGuid128 getTargetGuid() const ;
	
protected:
	WowUnitRace mRace;
	WowUnitClass mClass;
	int mLevel;
	long mHealth;
	long mMaxHealth;
	long mEnergy;
	long mMaxEnergy;
	bool mIsInCombat;
	bool mLootable;
	bool mTappedByOther;
	bool mTappedByMe;
	WowGuid128 mSummonedBy;
	WowGuid128 mTarget;
};

