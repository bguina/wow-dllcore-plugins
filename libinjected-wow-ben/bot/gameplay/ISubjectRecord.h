#pragma once

#include "snapshot/IBenWowGameSnapshot.h"

template<typename SubjectImage, typename Subject>
class IBenRecord {
public:
	using Timestamp = IBenWowGameSnapshot::Timestamp;
	
	static long long timestampDelta(const Timestamp a, const Timestamp b)
	{
		if (a > b) return (a - b);
		return -static_cast<long long>(b - a);
	}
	
	virtual ~IBenRecord() = default;

	virtual bool record(const Subject& game) = 0;

	virtual std::shared_ptr<const SubjectImage> getFrameAtTimestamp(Timestamp ms) const = 0;
	virtual std::shared_ptr<const SubjectImage> back() const = 0;
	virtual std::shared_ptr<const SubjectImage> previous() const = 0;
	virtual std::shared_ptr<const SubjectImage> front() const = 0;

	virtual size_t getRecordsCount() const = 0;
	virtual unsigned long long getRecordedDuration() const = 0;
	virtual unsigned long long getMaxRecordDuration() const = 0;
	virtual std::shared_ptr<const SubjectImage> operator[](const Timestamp timestamp) const = 0;

	virtual bool forget() = 0;
};
