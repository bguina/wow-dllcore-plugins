#pragma once

#include "game/WowGame.h"

class ISpellDefinition
{
public:
	virtual ~ISpellDefinition() = default;

	virtual int getRankId() const = 0;
	virtual int getSpellId() const = 0;
	virtual int getMaxRank() const = 0;
	virtual bool cast(WowGame& game, const WowGuid128* guid) = 0;
	virtual bool isOnCooldown(const WowGame& game) const = 0;
	virtual unsigned long getCooldownMs(const WowGame& game) const = 0;
	virtual unsigned long getMaxCooldownMs() const = 0;
};

template<const int Ranks[]>
class StaticSpellRanks {
public:

	int getRankSpellId(int rank) const
	{
		return Ranks[rank - 1];
	}

	int getMaxRank() const
	{
		return sizeof(Ranks);
	}

	int operator[](int index) const
	{
		return Ranks[index];
	}

	template<int RankNumber, unsigned long CooldownMs>
	class Rank final : public ISpellDefinition
	{
	public:
		explicit Rank() :
			mCooldownTs(0)
		{
		}

		int getRankId() const override
		{
			return RankNumber;
		}

		int getSpellId() const override
		{
			return Ranks[RankNumber - 1];
		}

		int getMaxRank() const override
		{
			return sizeof(Ranks);
		}

		bool cast(WowGame& game, const WowGuid128* guid) override
		{
			if (getCooldownMs(game) <= 0)
			{
				mCooldownTs = game.getTime() + getMaxCooldownMs();
				game.getSpellBook().castSpell(game, getSpellId(), guid);
				return true;
			}
			return false;
		}
		
		bool isOnCooldown(const WowGame& game) const override
		{
			return game.getSpellBook().getSpellCooldownTime(game, getSpellId()) > game.getTime();
		}
		
		unsigned long  getCooldownMs(const WowGame& game) const override
		{
			const auto cd(game.getSpellBook().getSpellCooldownTime(game, getSpellId()));
			if (cd < game.getTime())
				return 0;
			return (unsigned long)(cd - game.getTime());
		}

		unsigned long getMaxCooldownMs() const override
		{
			return CooldownMs;
		}


	protected:
		unsigned long long mCooldownTs;
	};

};
