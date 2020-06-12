#pragma once

#include <array>

#include "IWowGameRecord.h"

template<unsigned int DurationMs, unsigned int PeriodMs>
class AWowGameRecord : public IBenWowGameRecord
{
public:
	AWowGameRecord() :
		mDbg("AWowGameRecord"),
		mCursor(-1)
	{
		mSnapshots.fill(nullptr);
	}

	virtual ~AWowGameRecord() = default;

	bool record(const WowGame& game) override
	{
		const Timestamp currentTime(GetTickCount64());
		const auto lastFrame(front());

		if (nullptr == lastFrame || 1 == PeriodMs || currentTime >= lastFrame->getTimestamp() + PeriodMs)
		{
			const auto snapshot(makeSnapshot(game));

			if (nullptr == snapshot)
			{
				return false;
			}

			mCursor = (mCursor + 1) % BUFFER_SIZE;
			mSnapshots[mCursor] = snapshot;
		}

		return true;
	}

	std::shared_ptr<const IBenWowGameSnapshot> getFrameAtTimestamp(Timestamp timestamp) const override
	{
		if (nullptr == mSnapshots[0]) return nullptr;

		const auto lowerTimestamp(timestamp - PeriodMs / 2); // peek one frame before
		const typename SnapshotContainer::const_reverse_iterator ritLatest(mSnapshots.begin() + mCursor);
		const typename SnapshotContainer::const_reverse_iterator ritTail(mSnapshots.begin() + getRecordsCount());
		const auto p = [](const auto& x, const auto& y) { return x->getTimestamp() >= y->getTimestamp(); };
		const auto head(std::lower_bound(ritLatest, mSnapshots.rend(), std::make_shared<SnapshotTimestampSeeker>(lowerTimestamp), p));
		const auto tail(std::lower_bound(ritTail, ritLatest, std::make_shared<SnapshotTimestampSeeker>(lowerTimestamp), p));
		const auto frame(tail != ritLatest && (head == mSnapshots.rend() || timestamp - tail->get()->getTimestamp() < timestamp - head->get()->getTimestamp()) ? tail : head);

		if (frame == mSnapshots.rend()) return back();

		const auto index(std::distance(std::begin(mSnapshots), frame.base()) - 1);
		const auto nextIndex((index + 1) % getRecordsCount());

		if (nullptr != mSnapshots[nextIndex] && timestamp - mSnapshots[index]->getTimestamp() > PeriodMs / 2) return mSnapshots[nextIndex];
		return mSnapshots[index];
	}

	std::shared_ptr<const IBenWowGameSnapshot> back() const override
	{
		if (nullptr == mSnapshots[0]) return nullptr;
		return mSnapshots[(mCursor + 1) % getRecordsCount()];
	}

	std::shared_ptr<const IBenWowGameSnapshot> previous() const override
	{
		if (nullptr == mSnapshots[0]) return nullptr;

		const auto count(getRecordsCount());
		return mSnapshots[(mCursor + count - 1) % count];
	}

	std::shared_ptr<const IBenWowGameSnapshot> front() const override
	{
		if (nullptr == mSnapshots[0]) return nullptr;
		return mSnapshots[mCursor];
	}

	size_t getRecordsCount() const override
	{
		return nullptr != mSnapshots.back() ? BUFFER_SIZE : mCursor + 1;
	}

	unsigned long long getRecordedDuration() const override
	{
		if (nullptr == mSnapshots[0]) return 0;
		return front()->getTimestamp() - back()->getTimestamp();
	}

	unsigned long long getMaxRecordDuration() const override
	{
		return DurationMs;
	}

	std::shared_ptr<const IBenWowGameSnapshot> operator[](const Timestamp timestamp) const override
	{
		return getFrameAtTimestamp(timestamp);
	}


	bool forget() override
	{
		return false;
	}

protected:
	virtual std::shared_ptr<IBenWowGameSnapshot> makeSnapshot(const WowGame& game) = 0;
	
	static constexpr size_t BUFFER_SIZE = DurationMs / PeriodMs; // number of required pointer array entries of snapshots
	using SnapshotContainer = std::array<std::shared_ptr<IBenWowGameSnapshot>, BUFFER_SIZE>;

	FileLogger mDbg;
	SnapshotContainer mSnapshots;
	size_t mCursor;

	class SnapshotTimestampSeeker final : public IBenWowGameSnapshot
	{
	public:
		SnapshotTimestampSeeker(const long long seekTime) : mTime(seekTime) { }

		Timestamp getTimestamp() const override { return mTime; }

		// no need for this, use dummy implementation
		long getNetworkLatencyMs() const override { return 0; }
		
		const WowPlayerObject* getSelf() const override { return nullptr; }
		std::shared_ptr<const WowUnitSnapshot> getUnitByGuid(WowGuid128 guid) const override { return nullptr; }
		const UnitList& getUnitList(Faction faction) const override
		{
			return mDummyField;
		}

		bool isInGame() const override { return false; }
		bool isInCombat() const override { return false; }

	private:
		Timestamp mTime;
		UnitList mDummyField;
	};
};
