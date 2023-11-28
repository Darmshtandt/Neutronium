#include <istream>
#include <ostream>

#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Utilities.h>
#include <Nt/Core/Serialization.h>

namespace Nt {
	void Serialization::_StreamWrite(std::ostream& Stream, cString Data, const uInt& Size) {
		Stream.write(Data, Size);
	}
	void Serialization::_StreamRead(std::istream& Stream, Char* pData, const uInt& Size) {
		Stream.read(pData, Size);
	}
}