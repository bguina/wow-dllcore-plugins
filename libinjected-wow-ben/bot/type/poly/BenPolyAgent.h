#pragma once

#include "../base/ABenAgent.h"

class BenLoginAgent;

class BenPolyAgent : public ABenAgent
{
public:
	BenPolyAgent(BenLoginAgent* login, ABenAgent* characterSelector, ABenAgent* gameAgent);
	virtual ~BenPolyAgent();

	virtual void assignTask(ABenAgent* task);

protected:
	bool onEvaluatedIdle() override;
	
	std::unique_ptr<ABenAgent> mLogin;
	std::unique_ptr<ABenAgent> mCharacterSelector;
	std::unique_ptr<ABenAgent> mGameAgent;
};
