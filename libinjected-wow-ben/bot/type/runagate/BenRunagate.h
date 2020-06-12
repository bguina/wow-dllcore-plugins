#pragma once

#include "../champion/base/ABenChampion.h"

class BenRunagate : public ABenChampion
{
public:
	BenRunagate();
	BenRunagate(const std::shared_ptr<IBenWowGameEvaluator<WowBaseEvaluation>>& gameplay);
	virtual ~BenRunagate();

protected:
	bool onEvaluatedAsChampion() override;
};
