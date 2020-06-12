#pragma once

#include "AWowGameRecord.h"
#include "snapshot/BenGameSnapshot.h"

template<unsigned int DurationMs, unsigned int PeriodMs>
class BenGameRecord : public AWowGameRecord<DurationMs, PeriodMs>
{
public:
	
	BenGameRecord() : AWowGameRecord<DurationMs, PeriodMs>()
	{
	}

	virtual ~BenGameRecord() = default;

	std::shared_ptr<IBenWowGameSnapshot> makeSnapshot(const WowGame& game) override
	{
		return std::make_shared<BenGameSnapshot>(game);
	}
};

using BenWowGameBackBuffer = BenGameRecord<2, 1>;
