#pragma once

#include "injected/plugin/IDllPlugin.h"

class ExampleRemotePlugin : public IDllPlugin {
public:
	ExampleRemotePlugin();
	virtual ~ExampleRemotePlugin();

	virtual void onD3dRender() override;

protected:
};
