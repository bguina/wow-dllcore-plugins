#ifndef HEXDUMP_HPP
#define HEXDUMP_HPP

#include <cctype>
#include <iomanip>
#include <ostream>

template <unsigned RowSize, bool ShowAscii>
struct CustomHexdump
{
	CustomHexdump(const void* address, unsigned length) :
		mAddress(static_cast<const unsigned char*>(address)), mLength(length) { }
	const unsigned char* mAddress;
	const unsigned mLength;
};

template <unsigned RowSize, bool ShowAscii>
std::ostream& operator<<(std::ostream& out, const CustomHexdump<RowSize, ShowAscii>& dump)
{
	out.fill('0');
	for (int i = 0; i < dump.mLength; i += RowSize)
	{
		out << "0x" << std::setw(6) << std::hex << i << ": ";
		for (int j = 0; j < RowSize; ++j)
		{
			if (i + j < dump.mLength)
			{
				out << std::hex << std::setw(2) << static_cast<int>(dump.mAddress[i + j]) << " ";
			}
			else
			{
				out << "   ";
			}
		}

		out << " ";
		if (ShowAscii)
		{
			for (int j = 0; j < RowSize; ++j)
			{
				if (i + j < dump.mLength)
				{
					if (std::isprint(dump.mAddress[i + j]))
					{
						out << static_cast<char>(dump.mAddress[i + j]);
					}
					else
					{
						out << ".";
					}
				}
			}
		}
		out << std::endl;
	}
	return out;
}

typedef CustomHexdump<16, true> Hexdump;

#endif // HEXDUMP_HPP