#pragma once

#include "IBot.h"
#include "../../debugger/FileDebugger.h"

template<typename G>
class AGameBot : public IBot
{
public:
	AGameBot(G& game) :
		mGame(game),
		mDbg(game.getName() + "Bot")
	{

	}

	virtual ~AGameBot() {

	}

	const G& getGame() const {
		return mGame;
	}

	virtual void run() = 0;

protected:
	G& mGame;
	FileDebugger mDbg;
};
