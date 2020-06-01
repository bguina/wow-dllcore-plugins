#pragma once

/*
	Helpful reminder
	https://stackoverflow.com/questions/10334915/interface-vs-composition
*/

class WowGame;
class ServerWowMessage;

class IWowBot
{
public:
	virtual ~IWowBot() {};

	virtual const std::string& getTag() const = 0;

	virtual bool attach(std::shared_ptr<WowGame> game) = 0;

	virtual void onResume() = 0;
	virtual void onEvaluate() = 0;
	virtual void onPause() = 0;

	virtual bool handleWowMessage(ServerWowMessage& cl) = 0;
};
