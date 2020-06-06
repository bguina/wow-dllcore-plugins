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
		const auto lastFrame(getLastFrame());

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
		const auto latestIt(mSnapshots.begin() + mLastSnapshotIndex);
		const typename SnapshotContainer::const_reverse_iterator rBeginFromLatest(latestIt);
		const typename SnapshotContainer::const_reverse_iterator rBeginFromTail(mSnapshots.begin() + getRecordsCount());
		const auto predicate = [](const auto& x, const auto& y) { return x->getTimestamp() >= y->getTimestamp(); };
		const auto headResult(std::lower_bound(rBeginFromLatest, mSnapshots.rend(), std::make_shared<SnapshotTimestampSeeker>(lowerTimestamp), predicate));
		const auto tailResult(std::lower_bound(rBeginFromTail, rBeginFromLatest, std::make_shared<SnapshotTimestampSeeker>(lowerTimestamp), predicate));
		const auto nearest(tailResult != rBeginFromLatest && (headResult == mSnapshots.rend() || timestamp - tailResult->get()->getTimestamp() < timestamp - headResult->get()->getTimestamp()) ? tailResult : headResult);

		if (nearest == mSnapshots.rend()) return nullptr;

		const auto index(std::distance(std::begin(mSnapshots), nearest.base()) - 1);
		const auto nextIndex((index + 1) % getRecordsCount());
		const auto& frameBefore(mSnapshots[index]);
		const auto& frameAfter(mSnapshots[nextIndex]);

		if (nullptr != frameAfter && timestamp - frameBefore->getTimestamp() > mPeriodMilliseconds / 2) return mSnapshots[nextIndex];
		return mSnapshots[index];
	}

	std::shared_ptr<const IBenGameSnapshot> getLastFrame() const override
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
		const auto firstSnapshotIndex((mLastSnapshotIndex + BUFFER_SIZE - 1) % BUFFER_SIZE);
		const auto& firstSnapshot(mSnapshots[firstSnapshotIndex]);
		return getLastFrame()->getTimestamp() - firstSnapshot->getTimestamp();
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
		SnapshotTimestampSeeker(long long time) : mTime(time) {}

		Timestamp getTimestamp() const override { return mTime; }
		long getNetworkLatencyMs() const override { return 0; }
		const UnitList& listHostiles() const override { return mDummy; }
		const UnitList& listNonHostiles() const override { return mDummy; }
		const WowPlayerObject* getSelf() const override { return nullptr; }

	private:
		Timestamp mTime;
		UnitList mDummy;
	};
};
