#pragma once

#include "IBenGameExtrapolator.h"

class ABenGameExtrapolator : public IBenGameExtrapolator {
public:
	ABenGameExtrapolator();
	virtual ~ABenGameExtrapolator();

protected:
	std::vector<IBenGameSnapshot*> mHistory;
};
