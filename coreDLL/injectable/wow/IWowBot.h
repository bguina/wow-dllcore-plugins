#pragma once

/*
	Helpful reminder
	https://stackoverflow.com/questions/10334915/interface-vs-composition
*/

class WowGame;
class ClientMessage;

class IWowBot
{
public:
	virtual ~IWowBot() {};

	virtual const char* getTag() const = 0;

	// Game has objectmanager.getActivePlayer ready
	//virtual void _onGameplayResumed() = 0;
	//virtual void _onGameplayStopped() = 0;

	//virtual void _onTargetChanged() = 0;

	//virtual void _onCombatStart() = 0;
	//virtual void _onCombatEnd() = 0;

	virtual void onResume() = 0;
	virtual void onEvaluate() = 0;
	virtual void onPause() = 0;

	virtual bool handleServerMessage(ClientMessage& serverMessage) = 0;
};
