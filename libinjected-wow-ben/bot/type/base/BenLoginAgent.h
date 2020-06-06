#pragma once

#include "../base/ABenAgent.h"

class BenLoginAgent : public ABenAgent
{
public:
	BenLoginAgent(
		const std::string& login, 
		const std::string& password
	);
	virtual ~BenLoginAgent();

protected:
	const std::string mLogin;
	const std::string& mPassword;
};
