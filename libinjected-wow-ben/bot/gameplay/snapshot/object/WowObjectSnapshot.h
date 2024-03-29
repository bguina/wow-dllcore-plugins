#pragma once

#include "game/world/WowVector3f.h"
#include "game/world/object/WowGuid128.h"
#include "game/world/object/WowObject.h"

class WowGameObjectObject;

class WowObjectSnapshot
{
public:
	WowObjectSnapshot(const WowObject& obj);
	virtual ~WowObjectSnapshot();

	WowGuid128 getGuid() const ;
	const WowGuid128* getGuidPtr() const ;
	WowObjectType getType() const ;
	std::string getTypeLabel() const ;

	const WowVector3f& getPosition() const;
	float getX() const ;
	float getY() const ;
	float getZ() const ;
	float getFacingRadians() const ;
	
protected:
	WowGuid128 mGuid;
	WowVector3f mPosition;
	float mFacing;
};
