#pragma once

#include "../champion/base/ABenChampion.h"

class BenRunagate : public ABenChampion
{
public:
	BenRunagate();
	virtual ~BenRunagate();

protected:
	bool onEvaluatedInFight() override;
};
