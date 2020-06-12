#include "WowObjectSnapshot.h"

#include "game/world/object/WowGameObjectObject.h"
#include "game/world/object/WowObject.h"

WowObjectSnapshot::WowObjectSnapshot(const WowObject& obj) :
	mGuid(obj.getGuid()->upper(), obj.getGuid()->lower()),
	mPosition(obj.getPosition()),
	mFacing(0)
{
}

WowObjectSnapshot::~WowObjectSnapshot() = default;

WowGuid128 WowObjectSnapshot::getGuid() const
{
	return mGuid;
}

const WowVector3f& WowObjectSnapshot::getPosition() const
{
	return mPosition;
}

const WowGuid128* WowObjectSnapshot::getGuidPtr() const
{
	return &mGuid;
}

WowObjectType WowObjectSnapshot::getType() const
{
	return WowObjectType::Object;
}

std::string WowObjectSnapshot::getTypeLabel() const
{
	return "WowObjectSnapshot";
}

float WowObjectSnapshot::getX() const
{
	return mPosition.position.x;
}

float WowObjectSnapshot::getY() const
{
	return mPosition.position.y;
}

float WowObjectSnapshot::getZ() const
{
	return mPosition.position.z;
}

float WowObjectSnapshot::getFacingRadians() const
{
	return mFacing;
}
