#pragma once

#include <array>

#include "ABenGameplay.h"

template<unsigned int durationMs, unsigned int periodMs>
class BenRecordedGameplay : public ABenGameplay
{
public:
	BenRecordedGameplay(IBenGameSnapshotEvaluator* evaluator) :
		ABenGameplay(periodMs, evaluator),
		mDbg("BenRecordedGameplay"),
		mLastSnapshotIndex(-1)
	{
		mSnapshots.fill(nullptr);
	}

	virtual ~BenRecordedGameplay() = default;

	bool snap(const WowGame& game) override
	{
		const Timestamp currentTime(game.getTime());
		const auto lastFrame(front());

		if (nullptr == lastFrame || currentTime >= lastFrame->getTimestamp() + mPeriodMilliseconds)
		{
			const auto snapshot(takeSnapshot(game));

			if (nullptr == snapshot)
			{
				return false;
			}

			mLastSnapshotIndex = (mLastSnapshotIndex + 1) % BUFFER_SIZE;
			mSnapshots[mLastSnapshotIndex] = snapshot;
		}

		return true;
	}

	std::shared_ptr<const IBenGameSnapshot> getFrameAtTimestamp(Timestamp timestamp) const override
	{
		if (nullptr == mSnapshots[0]) return nullptr;

		const auto lowerTimestamp(timestamp - mPeriodMilliseconds / 2); // peek one frame before
		const typename SnapshotContainer::const_reverse_iterator ritLatest(mSnapshots.begin() + mLastSnapshotIndex);
		const typename SnapshotContainer::const_reverse_iterator ritTail(mSnapshots.begin() + getRecordsCount());
		const auto p = [](const auto& x, const auto& y) { return x->getTimestamp() >= y->getTimestamp(); };
		const auto head(std::lower_bound(ritLatest, mSnapshots.rend(), std::make_shared<SnapshotTimestampSeeker>(lowerTimestamp), p));
		const auto tail(std::lower_bound(ritTail, ritLatest, std::make_shared<SnapshotTimestampSeeker>(lowerTimestamp), p));
		const auto frame(tail != ritLatest && (head == mSnapshots.rend() || timestamp - tail->get()->getTimestamp() < timestamp - head->get()->getTimestamp()) ? tail : head);

		if (frame == mSnapshots.rend()) return back();

		const auto index(std::distance(std::begin(mSnapshots), frame.base()) - 1);
		const auto nextIndex((index + 1) % getRecordsCount());

		if (nullptr != mSnapshots[nextIndex] && timestamp - mSnapshots[index]->getTimestamp() > mPeriodMilliseconds / 2) return mSnapshots[nextIndex];
		return mSnapshots[index];
	}

	std::shared_ptr<const IBenGameSnapshot> back() const override
	{
		if (nullptr == mSnapshots[0]) return nullptr;
		return mSnapshots[(mLastSnapshotIndex + 1) % getRecordsCount()];
	}
	
	std::shared_ptr<const IBenGameSnapshot> previous() const override
	{
		if (nullptr == mSnapshots[0]) return nullptr;

		const auto count(getRecordsCount());
		return mSnapshots[(mLastSnapshotIndex + count - 1) % count];
	}
	
	std::shared_ptr<const IBenGameSnapshot> front() const override
	{
		if (nullptr == mSnapshots[0]) return nullptr;
		return mSnapshots[mLastSnapshotIndex];
	}

	size_t getRecordsCount() const override
	{
		return nullptr != mSnapshots.back() ? BUFFER_SIZE : mLastSnapshotIndex + 1;
	}

	unsigned long long getRecordedDuration() const override
	{
		if (nullptr == mSnapshots[0]) return 0;
		return front()->getTimestamp() - back()->getTimestamp();
	}

	unsigned long long getMaxRecordDuration() const override
	{
		return durationMs;
	}

	std::shared_ptr<const IBenGameSnapshot> operator[](const Timestamp timestamp) const override {
		return getFrameAtTimestamp(timestamp);
	}

protected:
	static constexpr size_t BUFFER_SIZE = durationMs / periodMs;
	using SnapshotContainer = std::array<std::shared_ptr<IBenGameSnapshot>, BUFFER_SIZE>;

	FileLogger mDbg;
	SnapshotContainer mSnapshots;
	size_t mLastSnapshotIndex;

	class SnapshotTimestampSeeker : public IBenGameSnapshot
	{
	public:
		SnapshotTimestampSeeker(const long long time) : mTime(time) {}

		Timestamp getTimestamp() const override { return mTime; }
		long getNetworkLatencyMs() const override { return 0; }
		const UnitList& getHostileList() const override { return mDummy; }
		const UnitList& getNonHostileList() const override { return mDummy; }
		const WowPlayerObject* getSelf() const override { return nullptr; }

		std::shared_ptr<const WowUnitSnapshot> getUnitByGuid(WowGuid128 guid) const override { return nullptr; }
	private:
		Timestamp mTime;
		UnitList mDummy;
	};
};
