#pragma once

#include "../base/ABenAgent.h"

class BenLoginAgent;

class BenPolyAgent : public ABenAgent
{
public:
	BenPolyAgent(BenLoginAgent* login, ABenAgent* characterSelector, IWowBot* gameAgent);
	virtual ~BenPolyAgent();

	virtual void assignTask(IWowBot* task);

protected:
	bool onEvaluatedIdle() override;
	
	std::unique_ptr<IWowBot> mLogin;
	std::unique_ptr<IWowBot> mCharacterSelector;
	std::unique_ptr<IWowBot> mGameAgent;
};
