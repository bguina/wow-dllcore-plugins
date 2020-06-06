
#include "BenLoginAgent.h"

const std::string TAG = "BenLoginAgent";

BenLoginAgent::BenLoginAgent(
	const std::string& login,
	const std::string& password
) :
	ABenAgent(nullptr, TAG),
	mLogin(login),
	mPassword(password)
{
}

BenLoginAgent::~BenLoginAgent() = default;

//void BenLoginAgent::onEvaluatedIdle() {
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
