#pragma once

class ISandboxPlugin
{
public:
	virtual ~ISandboxPlugin() {};
	virtual void run() = 0;
};
