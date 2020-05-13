#pragma once

#include <cctype>
#include <iomanip>
#include <ostream>

template <class T>
struct Hexsearch
{
	Hexsearch(const void * address, const T& data, unsigned length) :
		mAddress((static_cast<char const*>(address))), mData(reinterpret_cast<char const*>(&data)), mSz(sizeof(data)), mLength(length) { }
	const char const* mAddress;
	const char const* mData;
	const unsigned mSz;
	const unsigned mLength;
};

template <class T>
std::ostream& operator<<(std::ostream& out, const Hexsearch<T>& dump)
{
	for (int i = 0; i < dump.mLength; i++) {
		if (0 == memcmp(dump.mAddress + i, dump.mData, dump.mSz))
			out << "found result at +0x" << std::hex << i << std::endl;
	}
	out << std::endl;
	return out;
}
