#pragma once

template<class T>
class IServerObserver  {
public:
	virtual void capture(const T& client) = 0;
};
