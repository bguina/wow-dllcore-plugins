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

	virtual const char* getTag() const = 0;

	virtual void onResume(WowGame& game) = 0;
	virtual void onEvaluate(WowGame& game) = 0;
	virtual void onPause(WowGame& game) = 0;

	virtual bool handleWowMessage(ServerWowMessage& cl) = 0;
};
