#pragma once

namespace Nt {
	struct ISerialization {
		virtual void Write(std::ostream& Stream) const = 0;
		virtual void Read(std::istream& Stream) = 0;
		virtual constexpr uInt Sizeof() const noexcept = 0;
		virtual constexpr uInt ClassType() const noexcept = 0;

		static ISerialization* New(const uInt& ClassType) {
			return nullptr; 
		}
	};

// ============================================================================
//	Concepts
// ----------------------------------------------------------------------------
	template <typename _Ty>
	constexpr bool IsString = std::is_same_v<_Ty, std::string>;
	template <typename _Ty>
	constexpr bool IsWString = std::is_same_v<_Ty, std::wstring>;
	template <typename _Ty>
	constexpr bool IsCStr = std::is_same_v<_Ty, cString>;
	template <typename _Ty>
	constexpr bool IsCWStr = std::is_same_v<_Ty, cwString>;

	template <typename _Ty>
	constexpr bool IsPointer = std::is_pointer_v<_Ty>;
	template <typename _Ty>
	constexpr bool IsArray = std::is_array_v<_Ty>;
	template <typename _Ty>
	constexpr bool IsSerial = std::derived_from<_Ty, ISerialization>;

	template <typename _Ty>
	concept IsContainer = !IsString<_Ty> && !IsWString<_Ty> && !IsArray<_Ty> && requires(_Ty Container) {
		typename _Ty::value_type;
		typename _Ty::reference;
		typename _Ty::const_reference;
		typename _Ty::iterator;
		typename _Ty::const_iterator;
		typename _Ty::size_type;
		
		{ Container.begin() } -> std::convertible_to<typename _Ty::iterator>;
		{ Container.end() } -> std::convertible_to<typename _Ty::iterator>;
		{ Container.cbegin() } -> std::convertible_to<typename _Ty::const_iterator>;
		{ Container.cend() } -> std::convertible_to<typename _Ty::const_iterator>;
		{ Container.clear() } -> std::convertible_to<void>;
	};

	template <typename _Ty>
	concept IsHerePushBack = IsContainer<_Ty> && requires(_Ty Container, typename _Ty::value_type Value) {
		{ Container.push_back(Value) } -> std::convertible_to<void>;
	};
	template <typename _Ty>
	concept IsHerePushFront = IsContainer<_Ty> && requires(_Ty Container, typename _Ty::value_type Value) {
		{ Container.push_front(Value) } -> std::convertible_to<void>;
	};
	template <typename _Ty>
	concept IsHerePush = IsContainer<_Ty> && requires(_Ty Container, typename _Ty::value_type Value) {
		{ Container.push(Value) } -> std::convertible_to<void>;
	};
	template <typename _Ty>
	concept IsHereInsert = IsContainer<_Ty> && requires(_Ty Container, typename _Ty::value_type Value) {
		{ Container.insert(Value) } -> std::convertible_to<std::pair<typename _Ty::iterator, bool>>;
	};

// ============================================================================
//	Serialization
// ----------------------------------------------------------------------------
	class Serialization {
	public:
		template <typename... _Tys>
		__inline static void WriteAll(std::ostream& Stream, const _Tys&... Values) {
			_WriteRecursive(Stream, Values...);
		}
		template <typename... _Tys>
		__inline static void ReadAll(std::istream& Stream, _Tys&... Values) {
			_ReadRecursive(Stream, Values...);
		}

	private:
// ============================================================================
//	Functions
// ----------------------------------------------------------------------------
		template <typename _Ty> requires IsHerePushBack<_Ty>
		__inline static void _UniversalPush(_Ty& Container, const typename _Ty::value_type& Value) {
			Container.push_back(Value);
		}
		template <typename _Ty> requires (!IsHerePushBack<_Ty> && IsHerePushFront<_Ty>)
		__inline static void _UniversalPush(_Ty& Container, const typename _Ty::value_type& Value) {
			Container.push_front(Value);
		}
		template <typename _Ty> requires IsHerePush<_Ty>
		__inline static void _UniversalPush(_Ty& Container, const typename _Ty::value_type& Value) {
			Container.push(Value);
		}
		template <typename _Ty> requires (!IsHerePushBack<_Ty> && !IsHerePushFront<_Ty> && IsHereInsert<_Ty>)
		__inline static void _UniversalPush(_Ty& Container, const typename _Ty::value_type& Value) {
			Container.insert(Value);
		}

		NT_API static void _StreamWrite(std::ostream& Stream, cString Data, const uInt& Size);
		NT_API static void _StreamRead(std::istream& Stream, Char* pData, const uInt& Size);

	
// ============================================================================
//	Write
// ----------------------------------------------------------------------------
		template <typename _Ty>
		__inline static void _Write(std::ostream& Stream, _Ty Value) {
			_StreamWrite(Stream, reinterpret_cast<cString>(&Value), sizeof(Value));
		}
		template <typename _Ty> requires IsString<_Ty>
		__inline static void _Write(std::ostream& Stream, _Ty Value) {
			uInt Length = Value.length();
			_StreamWrite(Stream, reinterpret_cast<cString>(&Length), sizeof(Length));

			if (Length > 0)
				_StreamWrite(Stream, Value.data(), Length);
		}
		template <typename _Ty> requires IsWString<_Ty>
		__inline static void _Write(std::ostream& Stream, _Ty Value) {
			uInt Length = Value.length();
			_StreamWrite(Stream, reinterpret_cast<cString>(&Length), sizeof(Length));

			if (Length > 0)
				_StreamWrite(Stream, reinterpret_cast<cString>(Value.data()), Length * sizeof(wChar));
		}
		template <typename _Ty> requires IsCStr<_Ty>
		__inline static void _Write(std::ostream& Stream, _Ty& Value) {
			uInt Length = strlen(Value) + 1;
			_StreamWrite(Stream, reinterpret_cast<cString>(&Length), sizeof(Length));
			_StreamWrite(Stream, Value, Length);
		}
		template <typename _Ty> requires IsCWStr<_Ty>
		__inline static void _Write(std::ostream& Stream, _Ty& Value) {
			uInt Length = lstrlenW(Value) + 1;
			_StreamWrite(Stream, reinterpret_cast<cString>(&Length), sizeof(Length));
			_StreamWrite(Stream, reinterpret_cast<cString>(Value), Length * sizeof(wChar));
		}
		template <typename _Ty> requires IsArray<_Ty>
		__inline static void _Write(std::ostream& Stream, _Ty& Array) {
			uInt Size = ARRAYSIZE(Array);
			_StreamWrite(Stream, reinterpret_cast<cString>(&Size), sizeof(Size));

			if (Size > 0)
				for (auto& Value : Array)
					_Write(Stream, Value);
		}
		template <typename _Ty> requires (IsPointer<_Ty> && !IsArray<_Ty> && !IsCStr<_Ty> && !IsCWStr<_Ty> && !IsSerial<std::remove_pointer_t<_Ty>> && !std::is_abstract_v<std::remove_pointer_t<_Ty>>)
		__inline static void _Write(std::ostream& Stream, _Ty& Value) {
			uInt Size = sizeof(std::remove_pointer_t<_Ty>);
			_StreamWrite(Stream, reinterpret_cast<cString>(&Size), sizeof(Size));
			_Write(Stream, *Value);
		}
		template <typename _Ty> requires (IsPointer<_Ty> && !IsArray<_Ty> && !IsCStr<_Ty> && !IsCWStr<_Ty> && !IsSerial<std::remove_pointer_t<_Ty>>&& std::is_abstract_v<std::remove_pointer_t<_Ty>>)
		__inline static void _Write(std::ostream& Stream, _Ty Value) {
			Raise(L"Attempt write abstract class");
		}
		template <typename _Ty> requires IsSerial<_Ty>
		__inline static void _Write(std::ostream& Stream, _Ty Value) {
			Value.Write(Stream);
		}
		template <typename _Ty> requires IsSerial<std::remove_pointer_t<_Ty>> && IsPointer<_Ty>
		__inline static void _Write(std::ostream& Stream, _Ty Value) {
			uInt Type = 0;
			uInt Size = 0;
			if (Value) {
				Type = Value->ClassType();
				Size = Value->Sizeof() - sizeof(ISerialization);
			}

			_StreamWrite(Stream, reinterpret_cast<cString>(&Type), sizeof(Type));
			_StreamWrite(Stream, reinterpret_cast<cString>(&Size), sizeof(Size));
			if (Value) {
				//_StreamWrite(Stream, reinterpret_cast<cString>(Value) + sizeof(ISerialization), Size);
				Value->Write(Stream);
			}
		}
		template <typename _Ty> requires IsContainer<_Ty>
		__inline static void _Write(std::ostream& Stream, _Ty Container) {
			uInt Size = static_cast<uInt>(std::distance(Container.cbegin(), Container.cend()));
			_StreamWrite(Stream, reinterpret_cast<cString>(&Size), sizeof(Size));

			if (Size > 0)
				for (auto Value : Container)
					_Write(Stream, Value);
		}

		template <typename _Ty>
		__inline static void _WriteRecursive(std::ostream& Stream, _Ty Value) {
			_Write(Stream, Value);
		}
		template <typename _Ty, typename... _Tys>
		__inline static void _WriteRecursive(std::ostream& Stream, _Ty Value, _Tys... Values) {
			_Write(Stream, Value);
			_WriteRecursive(Stream, Values...);
		}


// ============================================================================
//	Read
// ----------------------------------------------------------------------------
		template <typename _Ty>
		__inline static void _Read(std::istream& Stream, _Ty& Value) {
			_StreamRead(Stream, (Char*)(&Value), sizeof(Value));
		}
		template <typename _Ty> requires IsString<_Ty>
		__inline static void _Read(std::istream& Stream, _Ty& Value) {
			uInt Length;
			_StreamRead(Stream, reinterpret_cast<Char*>(&Length), sizeof(Length));
			Value.resize(Length);
			_StreamRead(Stream, Value.data(), Length);
		}
		template <typename _Ty> requires IsWString<_Ty>
		__inline static void _Read(std::istream& Stream, _Ty& Value) {
			uInt Length;
			_StreamRead(Stream, reinterpret_cast<Char*>(&Length), sizeof(Length));
			if (Length == 0)
				return;

			Value.resize(Length);
			_StreamRead(Stream, reinterpret_cast<Char*>(Value.data()), Length * sizeof(wChar));
		}
		template <typename _Ty> requires IsCStr<_Ty>
		__inline static void _Read(std::istream& Stream, _Ty& Value) {
			uInt Length;
			_StreamRead(Stream, reinterpret_cast<Char*>(&Length), sizeof(Length));
			if (Length == 0)
				return;

			Char* Str = new char[Length];
			_StreamRead(Stream, Str, Length);
			Value = Str;
		}
		template <typename _Ty> requires IsCWStr<_Ty>
		__inline static void _Read(std::istream& Stream, _Ty& Value) {
			uInt Length;
			_StreamRead(Stream, reinterpret_cast<Char*>(&Length), sizeof(Length));

			wChar* wStr = new wChar[Length];
			_StreamRead(Stream, reinterpret_cast<Char*>(wStr), Length * sizeof(wChar));
			Value = wStr;
		}
		template <typename _Ty> requires IsArray<_Ty>
		__inline static void _Read(std::istream& Stream, _Ty& Array) {
			uInt Size;
			_StreamRead(Stream, reinterpret_cast<Char*>(&Size), sizeof(Size));

			for (uInt i = 0; i < Size; ++i)
				_Read(Stream, Array[i]);
		}
		template <typename _Ty> requires (IsPointer<_Ty> && !IsArray<_Ty> && !IsCStr<_Ty> && !IsCWStr<_Ty> && !IsSerial<std::remove_pointer_t<_Ty>>)
		__inline static void _Read(std::istream& Stream, _Ty& Value) {
			uInt Size;
			_StreamRead(Stream, reinterpret_cast<Char*>(&Size), sizeof(Size));
			Value = reinterpret_cast<_Ty>(malloc(Size));
			_Read(Stream, *Value);
		}
		template <typename _Ty> requires IsSerial<_Ty>
		__inline static void _Read(std::istream& Stream, _Ty& Value) {
			Value.Read(Stream);
		}
		template <typename _Ty> requires IsSerial<std::remove_pointer_t<_Ty>> && IsPointer<_Ty>
		__inline static void _Read(std::istream& Stream, _Ty& Value) {
			uInt Type;
			_StreamRead(Stream, reinterpret_cast<Char*>(&Type), sizeof(Type));
			Value = reinterpret_cast<_Ty>(std::remove_pointer_t<_Ty>::New(Type));

			uInt Size;
			_StreamRead(Stream, reinterpret_cast<Char*>(&Size), sizeof(Size));

			if (Size) {
				//_StreamRead(Stream, reinterpret_cast<Char*>(Value) + sizeof(ISerialization), Size);
				Value->Read(Stream);
			}
			else {
				SAFE_DELETE(&Value);
			}
		}
		template <typename _Ty> requires IsContainer<_Ty>
		__inline static void _Read(std::istream& Stream, _Ty& Container) {
			Int Size;
			_StreamRead(Stream, reinterpret_cast<Char*>(&Size), sizeof(Size));

			Container.clear();
			for (Int i = 0; i < Size; ++i) {
				typename _Ty::value_type Value = { };
				_Read(Stream, Value);
				_UniversalPush(Container, Value);
			}
		}

		template <typename _Ty>
		__inline static void _ReadRecursive(std::istream& Stream, _Ty& Value) {
			_Read(Stream, Value);
		}
		template <typename _Ty, typename... _Tys>
		__inline static void _ReadRecursive(std::istream& Stream, _Ty& Value, _Tys&... Values) {
			_Read(Stream, Value);
			_ReadRecursive(Stream, Values...);
		}
	};
}