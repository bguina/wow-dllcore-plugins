#include "pch.h"
#include "SpellBookManager.h"


SpellBookManager::SpellBookManager(
	const uint8_t* baseAddr
) :
	mPointerAddr(baseAddr)
{
	//get All spell IDS
	SpellbookDescriptor** ptrArray = *((SpellbookDescriptor***)(mPointerAddr - 0x20));
	uint32_t totalSpell = getSpellBookCount();

	for (size_t i = 0; i < totalSpell; i++)
	{
		SpellbookDescriptor* descriptor = ptrArray[i];
		if (descriptor == NULL)
			break;
	}
}

uint32_t SpellBookManager::getSpellBookCount() const {
	return *(uint32_t*)(mPointerAddr - 0x28);
}

void SpellBookManager::scan() {
	//DO SOMETHING ???
}

const uint8_t* SpellBookManager::getBaseAddress() const {
	return mPointerAddr;
}