#pragma once

class ISpell {
public:
	virtual ISpellRank getRankId(int rank) const = 0;
	virtual ISpellRank getMaxRank() const = 0;
};

class ISpellRank : public ISpell {
public:
	virtual int getRankId() const = 0;
	virtual ISpellRank getMaxRank() const = 0;
};

//template<const int ranks[], int index>
//class SpellRank : public Spell<ranks> {
//public:
//	SpellRank(ISpell spell) : mSpell(spell) {
//
//	}
//
//	virtual int getRankId() const override {
//		return ranks[index];
//	}
//	virtual ISpellRank getMaxRank() const override {
//		return ranks[sizeOf(ranks)];
//	}
//
//	ISpell mSpell;
//};

template<const int ranks[]>
class Spell : public ISpell {
public:
	Spell() :
		mRanks(ranks) {}

	virtual ISpellRank<ranks, rank> getRankId(int rank) const override {
		return SpellRank<ranks, rank>();
	}

	virtual ISpellRank<ranks, sizeof(ranks)> getMaxRank() const override {
		return SpellRank<ranks, sizeof(ranks)>();
	}

	const int mRanks[];
};

//static constexpr int spellFireballRanks[] = { 1,2,3 };
//
//
//using Fireball1 = SpellRank<spellFireballRanks, 0>;
//using Fireball2 = SpellRank<spellFireballRanks, 1>;
//using Fireball3 = SpellRank<spellFireballRanks, 2>;
//
//
////static constexpr SpellRank<spellFireballRanks, > Fireballs[] = { 1,2,3 };
//
//
//
//using Fireball = Spell < spellFireballRanks> ;
//static SpellRank<spellFireballRanks, 0> Fireball1 = Fireball();
