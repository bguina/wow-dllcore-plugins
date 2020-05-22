#pragma once

#include <sstream>

#include "../../MemoryObject.h"

#include "../dump/WowGameDescriptors.h"
#include "../WowVector3f.h"

typedef uint64_t WowGuid64;

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

	const uint8_t* getDescriptor() const;

	WowGuid64 getGuid() const;

	WowGuid64 getGuid2() const;

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
	void* vtableAt(unsigned index);
};

//typedef bool __thiscall (*CGPlayer_C__ClickToMove)(int *, int, int *, int *, float);

inline std::ostream& operator<<(
	std::ostream& out,
	const WowObject& obj
	)
{
	out << "[WowObject@" << (void*)obj.getAddress() << "]" << std::endl;
	out << obj.getTypeLabel() << "[GUID 0x" << (void*)obj.getGuid() << " | " << (void*)obj.getGuid2() << "]@" << obj.getX() << "," << obj.getY() << " facing " << obj.getFacingRadians();
	return out;
}

