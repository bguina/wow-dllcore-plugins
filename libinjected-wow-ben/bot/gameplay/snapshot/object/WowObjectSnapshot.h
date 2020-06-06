#pragma once

#include "game/WowVector3f.h"
#include "game/object/WowGuid128.h"
#include "game/object/WowObject.h"

class WowObjectSnapshot
{
public:
	WowObjectSnapshot(const WowObject& obj);
	virtual ~WowObjectSnapshot();

	WowGuid128 getGuid() const ;
	const WowGuid128* getGuidPtr() const ;
	WowObjectType getType() const ;
	std::string getTypeLabel() const ;

	const WowVector3f& getPosition() const ;
	float getX() const ;
	float getY() const ;
	float getZ() const ;
	float getFacingRadians() const ;
	
protected:
	WowGuid128 mGuid;
	WowVector3f mPosition;
	float mFacing;
};
