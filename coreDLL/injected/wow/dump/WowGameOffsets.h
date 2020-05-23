#pragma once

#include <cstdint>

struct WowGameOffsets {
	struct WowObjectManager {
		static const uint64_t OffsetFirstObject = 0x18;
		static const uint64_t OffsetNextObject = 0x70;
	};

	struct WowObject {
		static const uint64_t OffsetDescriptorPointer = 0x10;
		static const uint64_t OffsetType = 0x20;
		static const uint64_t OffsetGuid = 0x58;
		static const uint64_t OffsetPosition = 0x1600;
		static const uint64_t OffsetFacing = 0x1610;
		
	};

	struct WowUnitObject {
		static const uint64_t DescriptorOffsetClass = 0xD1;
		static const uint64_t DescriptorOffsetRace = 0x158;
		static const uint64_t DescriptorOffsetLevel = 0x134;
		static const uint64_t DescriptorOffsetHealth = 0xDC;
		static const uint64_t DescriptorOffsetMaxHealth = 0xFC;
		static const uint64_t DescriptorOffsetEnergy = 0xE4;
		static const uint64_t DescriptorOffsetMaxEnergy = 0x104;
		static const uint64_t DescriptorOffsetTargetGuid = 0x9C;
	};
};
