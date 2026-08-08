// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/Serialization.h>

namespace Nt {
	void Serialization::_StreamWrite(std::ostream& Stream, cString Data, const uInt& Size) {
		Stream.write(Data, Size);
	}
	void Serialization::_StreamRead(std::istream& Stream, Char* pData, const uInt& Size) {
		Stream.read(pData, Size);
	}
}