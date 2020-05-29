#pragma once

class ISpell {
public:
	virtual int getRankId(int rank) const = 0;
	virtual int getMaxRank() const = 0;
};

template<const int ranks[]>
class Spell : public ISpell {
public:
	int getRankId(int rank) const override {
		return ranks[rank];
	}

	int getMaxRank() const override {
		return sizeof(ranks);
	}
};

static constexpr int spellFireballRanks[] = { 1,2,3 };

using Fireball = Spell<spellFireballRanks>;
