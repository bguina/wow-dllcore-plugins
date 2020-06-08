#pragma once

#include "snapshot/IBenGameSnapshot.h"

class IBenGameRecord {
public:
	using Timestamp = IBenGameSnapshot::Timestamp;
	
	static long long timestampDelta(const Timestamp a, const Timestamp b)
	{
		if (a > b) return (a - b);
		return -static_cast<long long>(b - a);
	}
	
	virtual ~IBenGameRecord() = default;

	virtual bool record(const WowGame& game) = 0;

	virtual std::shared_ptr<const IBenGameSnapshot> getFrameAtTimestamp(Timestamp ms) const = 0;
	virtual std::shared_ptr<const IBenGameSnapshot> back() const = 0;
	virtual std::shared_ptr<const IBenGameSnapshot> previous() const = 0;
	virtual std::shared_ptr<const IBenGameSnapshot> front() const = 0;

	virtual size_t getRecordsCount() const = 0;
	virtual unsigned long long getRecordedDuration() const = 0;
	virtual unsigned long long getMaxRecordDuration() const = 0;
	virtual std::shared_ptr<const IBenGameSnapshot> operator[](const Timestamp timestamp) const = 0;

	virtual bool forget() = 0;
};
