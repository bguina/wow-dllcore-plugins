#pragma once

#include <cstdint>

class MemoryObject {
public:
	MemoryObject(const uint8_t* baseAddr);
	virtual ~MemoryObject();

	const uint8_t* getAddress() const;

	void rebase(const uint8_t* baseAddr);

	bool isPresent() const;

	template<typename T>
	const T& getFunction(uint64_t offset) const {
		return *reinterpret_cast<T*>(getAddress() + offset);
	}

	template<typename T>
	const T& get(uint64_t offset) const {
		return *reinterpret_cast<const T*>(getAddress() + offset);
	}

	template<typename T>
	T* getWritable(uint64_t offset) {
		return reinterpret_cast<T*>(getAddress() + offset);
	}

private:
	const uint8_t* mBaseAddr;
};
