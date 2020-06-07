#pragma once

#include "snapshot/IBenGameSnapshot.h"

class IBenGameplay {
public:
	using Timestamp = IBenGameSnapshot::Timestamp;
	static long long timestampDelta(Timestamp a, Timestamp b)
	{
		if (a > b) return (a - b);
		return -static_cast<long long>(b - a);
	}
	
	virtual ~IBenGameplay() = default;

	virtual unsigned int getPeriodMilliseconds() const = 0;
	virtual void setPeriodMilliseconds(unsigned int fps) = 0;
	
	virtual bool snap(const WowGame& game) = 0;

	virtual std::shared_ptr<const IBenGameSnapshot> getFrameAtTimestamp(Timestamp ms) const = 0;
	virtual std::shared_ptr<const IBenGameSnapshot> back() const = 0;
	virtual std::shared_ptr<const IBenGameSnapshot> previous() const = 0;
	virtual std::shared_ptr<const IBenGameSnapshot> front() const = 0;

	virtual size_t getRecordsCount() const = 0;
	virtual unsigned long long getRecordedDuration() const = 0;
	virtual unsigned long long getMaxRecordDuration() const = 0;

	virtual std::shared_ptr<const IBenGameSnapshot> operator[](const Timestamp timestamp) const = 0;
};
