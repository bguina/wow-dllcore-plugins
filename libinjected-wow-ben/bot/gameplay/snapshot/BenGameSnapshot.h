#pragma once

#include "ABenGameSnapshot.h"

class BenGameSnapshot final : public ABenGameSnapshot  {
public:
	
	BenGameSnapshot(const WowGame& game);
	virtual ~BenGameSnapshot();

	long getNetworkLatencyMs() const override;

	const UnitList& listHostiles() const override;
	const UnitList& listNonHostiles() const override;

protected:
	/**
	 *  https://www.bfilipek.com/2014/05/vector-of-objects-vs-vector-of-pointers.html
	 */
	UnitList mHostileUnits;
	UnitList mNonHostileUnits;
};
