#pragma once

template<class T>
class IGameObserver  {
public:
	virtual void capture(const T& game) = 0;
};
