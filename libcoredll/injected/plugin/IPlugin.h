#pragma once

class IPlugin
{
public:
	virtual ~IPlugin() {};
	virtual bool onD3dRender() = 0;
};
