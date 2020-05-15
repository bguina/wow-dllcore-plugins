#pragma once

template<typename T>
class Singleton
{
public:
	static T& getInstance() {
		static T instance{ madeSingleton{} };
		return instance;
	}

public:
	Singleton(Singleton const&) = delete;
	void operator=(Singleton const&) = delete;

protected:
	// token trick as seen at https://codereview.stackexchange.com/a/173942
	struct madeSingleton {};

	Singleton() {}
};
