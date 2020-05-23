#pragma once

#include <set>

#include "../ISandboxPlugin.h"

class Bot : public ISandboxPlugin
{
public:
	Bot();
	virtual ~Bot();

	virtual void pause(bool paused);
	virtual bool isPaused() const;

	virtual void run() = 0;

protected:
	bool mPaused;
};
