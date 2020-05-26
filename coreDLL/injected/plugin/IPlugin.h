#pragma once

class IPlugin
{
public:
	virtual ~IPlugin() {};
	virtual void onD3dRender() = 0;
};
