#pragma once

#include "../base/ABen.h"

class BenLogin : public ABen
{
public:
	BenLogin(
		const std::string login, 
		const std::string& password
	);
	virtual ~BenLogin();


protected:
	const std::string mLogin;
	const std::string& mPassword;
};
