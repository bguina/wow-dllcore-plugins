#pragma once

#include <sstream>

#include "../../../MemoryObject.h"

#include "../dump/WowGameDescriptors.h"
#include "../WowVector3f.h"

#include "WowGuid128.h"

class WowObject : public MemoryObject
{
public:
	WowObject(const uint8_t* baseAddr);

	template<class T>
	const T& downcast() const {
		return static_cast<const T&>(*this);
	}

	template<class T>
	T& downcast() {
		return static_cast<T&>(*this);
	}


	bool vanished() const;

	WowGuid128 getGuid() const;

	WowGuid128* getGuidPtr() const;

	//		StorageField = 0x10,//good-33526
	//		ObjectType = 0x20,//good-33526
	//		NextObject = 0x70,//good-33526
	//		FirstObject = 0x18,//good-33526
	//		LocalGUID = 0x58, //good-33526

	WowObjectType getType() const;

	std::string getTypeLabel() const;

	const WowVector3f& getPosition() const;

	float getX() const;
	float getY() const;
	float getZ() const;

	float getFacingRadians() const;

	int getFacingDegrees() const;

	// Position helpers
	float getDistanceTo(const WowObject& object) const;
	float getFlightDistanceTo(const WowObject& object) const;

	int getFacingDegreesTo(const WowObject& object) const;
	int getFacingDeltaDegrees(const WowObject& object) const;

protected:
	const uint8_t* getDescriptor() const;

	void* vtableAt(unsigned index);
};


inline std::ostream& operator<<(
	std::ostream& out,
	const WowObject& obj
	)
{
	out << "[" << obj.getTypeLabel() << ":" << (void*)obj.getAddress() << ":GUID " << obj.getGuid().upper() << obj.getGuid().lower() << ":Pos" << obj.getX() << "," << obj.getY() << "]";
	return out;
}
