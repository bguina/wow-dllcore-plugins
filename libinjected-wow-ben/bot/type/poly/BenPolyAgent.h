#pragma once

#include "../base/ABenAgent.h"

class BenLoginAgent;

class BenPolyAgent : public ABenAgent
{
public:
	BenPolyAgent(BenLoginAgent* login, ABenAgent* characterSelector, ABenAgent* task);
	virtual ~BenPolyAgent();

	virtual bool onEvaluatedIdle() override;

	virtual void assignTask(IWowBot* task);

protected:
	std::unique_ptr<IWowBot> mLogin;
	std::unique_ptr<IWowBot> mCharacterSelector;
	std::unique_ptr<IWowBot> mTask;
};
