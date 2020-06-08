#pragma once

#include "ABenGameRecord.h"
#include "snapshot/BenGameSnapshot.h"

template<unsigned int DurationMs, unsigned int PeriodMs>
class BenGameRecord : public ABenGameRecord<DurationMs, PeriodMs>
{
public:
	BenGameRecord() : ABenGameRecord<DurationMs, PeriodMs>()
	{
	}

	virtual ~BenGameRecord() = default;

	std::shared_ptr<IBenGameSnapshot> makeSnapshot(const WowGame& game) override
	{
		return std::make_shared<BenGameSnapshot>(game);
	}
};
