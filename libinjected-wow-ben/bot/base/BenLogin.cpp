
#include "BenLogin.h"

const std::string TAG = "BenLogin";

BenLogin::BenLogin(
	const std::string login,
	const std::string& password
) :
	ABen(TAG),
	mLogin(login),
	mPassword(password)
{
}

BenLogin::~BenLogin()
{
}

//void BenLogin::onEvaluate() {
	// todo login task
	//bool mainScreen = false;
	//bool connecting = false;

	//if (mainScreen) {
		// input(login)
		// input(tabKey)
		// input(password)
		// login
	//}
//}
