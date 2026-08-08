#include <Nt/Core/Math/Vector.h>

namespace Nt {
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size>::Vector() noexcept {
		if NT_CONSTEXPR (size >= 2 && size <= 4) {
			this->x = 0;
			this->y = 0;

			if NT_CONSTEXPR (size >= 3)
				this->z = 0;
			if NT_CONSTEXPR (size == 4)
				this->w = 0;
		}
		else {
			this->Array = { };
		}
	}

	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size>::Vector(const Vector& other) noexcept {
		if NT_CONSTEXPR (size >= 2 && size <= 4) {
			this->x = other.x;
			this->y = other.y;

			if NT_CONSTEXPR (size >= 3)
				this->z = other.z;
			if NT_CONSTEXPR (size == 4)
				this->w = other.w;
		}
		else {
			this->Array = other.Array;
		}
	}

	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size>::Vector(const std::initializer_list<_Ty>& list) noexcept {
		Assign<0>(list);
	}

	template <typename _Ty, uInt size>
	template <typename _U>
	NT_CONSTEXPR Vector<_Ty, size>::Vector(const Vector<_U, size>& other) noexcept {
		if NT_CONSTEXPR (size >= 2 && size <= 4) {
			this->x = other.x;
			this->y = other.y;

			if NT_CONSTEXPR (size >= 3)
				this->z = other.z;
			if NT_CONSTEXPR (size == 4)
				this->w = other.w;
		}
		else {
			this->Array = other.Array;
		}
	}

	template <typename _Ty, uInt size>
	template <typename... _Args, Requires(sizeof...(_Args) == size && all_same_v<_Args...>)>
	NT_CONSTEXPR Vector<_Ty, size>::Vector(_Args... args) noexcept {
		Assign<0>(args...);
	}


	template <typename _Ty, uInt size>
	template <uInt S, typename>
	NT_CONSTEXPR Vector<_Ty, size> Vector<_Ty, size>::GetCross(const Vector& vector) const noexcept {
		Vector result;
		result.x = (this->y * vector.z - this->z * vector.y);
		result.y = (this->z * vector.x - this->x * vector.z);
		result.z = (this->x * vector.y - this->y * vector.x);
		return result;
	}

	template <typename _Ty, uInt size>
	template <uInt index>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size>& Vector<_Ty, size>::Assign() noexcept {
		return (*this);
	}

	template <typename _Ty, uInt size>
	template <uInt index>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size>& Vector<_Ty, size>::Assign(const std::initializer_list<_Ty>& list) noexcept {
		if NT_CONSTEXPR(index < size) {
			if (index >= list.size())
				return (*this);

			auto it = list.begin();
			std::advance(it, index);
			Set<index>(*it);

			return Assign<index + 1>(list);
		}
		else {
			return (*this);
		}
	}

	template <typename _Ty, uInt size>
	template <uInt index, typename First, typename... Rest>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size>& Vector<_Ty, size>::Assign(First value, Rest... rest) noexcept {
		Set<index>(static_cast<_Ty>(value));
		return Assign<index + 1>(rest...);
	}

	template <typename _Ty, uInt size>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size>& Vector<_Ty, size>::Fill(const _Ty& scalar) noexcept {
		if NT_CONSTEXPR(size >= 2 && size <= 4) {
			this->x = scalar;
			this->y = scalar;

			if NT_CONSTEXPR(size >= 3)
				this->z = scalar;
			if NT_CONSTEXPR(size == 4)
				this->w = scalar;
		}
		else {
			for (uInt i = 0; i < Size; ++i)
				this->Array[i] = scalar;
		}
		return (*this);
	}

	template <typename _Ty, uInt size>
	template <uInt index>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size>& Vector<_Ty, size>::Set(const _Ty& scalar) {
		static_assert(index < size, "Out of range");

		if NT_CONSTEXPR (size >= 2 && size <= 4) {
			if NT_CONSTEXPR(index == 0)
				this->x = scalar;
			if NT_CONSTEXPR(index == 1)
				this->y = scalar;
			if NT_CONSTEXPR(index == 2)
				this->z = scalar;
			if NT_CONSTEXPR(index == 3)
				this->w = scalar;
		}
		else {
			this->Array[index] = scalar;
		}
		return (*this);
	}

	template <typename _Ty, uInt size>
	template <uInt index>
	NT_FORCE_INLINE NT_CONSTEXPR 
	_Ty Vector<_Ty, size>::Get() const {
		static_assert(index < size, "Out of range");

		if NT_CONSTEXPR(size >= 2 && size <= 4) {
			if NT_CONSTEXPR(index == 0)
				return this->x;
			if NT_CONSTEXPR(index == 1)
				return this->y;
			if NT_CONSTEXPR(index == 2)
				return this->z;
			if NT_CONSTEXPR(index == 3)
				return this->w;
		}
		else {
			return this->Array[index];
		}
	}


	template <typename _Ty, uInt size>
	template <uInt index>
	NT_CONSTEXPR _Ty Vector<_Ty, size>::LengthSquareImpl() const noexcept {
		if NT_CONSTEXPR (index < size)
			return Get<index>() * Get<index>() + LengthSquareImpl<index + 1>();
		else
			return static_cast<_Ty>(0);
	}
	
	template <typename _Ty, uInt size>
	template <uInt index>
	NT_CONSTEXPR _Ty Vector<_Ty, size>::DotImpl() const noexcept {
		return static_cast<_Ty>(0);
	}

	template <typename _Ty, uInt size>
	template <uInt index, typename First, typename... Rest>
	NT_CONSTEXPR _Ty Vector<_Ty, size>::DotImpl(First value, Rest... rest) const noexcept {
		static_assert(index < size, "Out of range");
		return Get<index>() * static_cast<_Ty>(value) + DotImpl<index + 1>(rest...);
	}


	template <typename _Ty, uInt size>
	NT_CONSTEXPR _Ty Vector<_Ty, size>::LengthSquare() const noexcept {
		return LengthSquareImpl<0>();
	}

	template <typename _Ty, uInt size>
	NT_CONSTEXPR _Ty Vector<_Ty, size>::Length() const noexcept {
		return static_cast<_Ty>(std::sqrt(static_cast<Double>(LengthSquare())));
	}

	template <typename _Ty, uInt size>
	_Ty Vector<_Ty, size>::GetDistance(const Vector& vector) const noexcept {
		return ApplyCopy<BinOpSub<_Ty>>(vector).Length();
	}

	template <typename _Ty, uInt size>
	NT_CONSTEXPR _Ty Vector<_Ty, size>::GetSquareDistance(const Vector& vector) const noexcept {
		return ApplyCopy<BinOpSub<_Ty>>(vector).LengthSquare();
	}

	template <typename _Ty, uInt size>
	Vector<_Ty, size> Vector<_Ty, size>::GetNormalize() const noexcept {
		const _Ty length = Length();
		return ApplyCopy<BinOpDiv<_Ty>>(length);
	}

	template <typename _Ty, uInt size>
	NT_CONSTEXPR _Ty Vector<_Ty, size>::Dot(const Vector& vector) const noexcept {
		if NT_CONSTEXPR (size >= 2 && size <= 4) {
			if NT_CONSTEXPR (size == 2)
				return DotImpl<0>(vector.x, vector.y);
			else if NT_CONSTEXPR (size == 3)
				return DotImpl<0>(vector.x, vector.y, vector.z);
			else if NT_CONSTEXPR (size == 4)
				return DotImpl<0>(vector.x, vector.y, vector.z, vector.w);
		}
		else {
			_Ty dot = 0;
			for (uInt i = 0; i < Size; ++i)
				dot += this->Array[i] * vector.Array[i];

			return dot;
		}
	}

	template <typename _Ty, uInt size>
	String Vector<_Ty, size>::ToString() const noexcept {
		NT_CONSTEXPR uInt literalCount = ('z' - 'a');

		String str = "{ ";
		for (uInt i = 0; i < Size; ++i) {
			if (i > 0 && (i + 1) < Size)
				str << ", ";

			Char literalCoordinates = 'x';
			if (i % literalCount < 3)
				literalCoordinates = Char(uInt('x') + i);
			else
				literalCoordinates = Char(uInt('w') - (i - 3));

			str << String(literalCoordinates) << ": " << String(this->Array[i]);
		}

		return str << " };";
	}

	template<typename _Ty, uInt size>
	Vector<_Ty, size>& Vector<_Ty, size>::FromString(const String& string) const noexcept {
		const uInt startIndex = string.find('{');
		const uInt endIndex = string.find("};");

		if (startIndex == uInt(-1) || endIndex == uInt(-1))
			return (*this);

		String vectorString = string.substr(startIndex + 1, endIndex - 2);
		vectorString.erase(std::remove_if(vectorString.begin(), vectorString.end(),
			[] (const Char& symbol) {
				return !(std::isalpha(symbol) || symbol == ',');
			}), vectorString.end());

		NT_CONSTEXPR uInt literalCount = ('z' - 'a');

		const std::vector<String> allCoords = string.Split(',');
		for (const String& coord : allCoords) {
			if (coord.empty())
				continue;

			const std::vector<String> data = coord.Split(':');
			if (data.size() != 2)
				continue;

			const Char litera = data[0];
			if (litera < 'a' || litera > 'z')
				continue;

			uInt index;
			if (Byte(litera) >= 'x')
				index = uInt(litera) - uInt('x');
			else
				index = literalCount - 1 - (uInt(litera) - uInt('a'));

			this->Array[index] = (_Ty)data[1].ToFloat();
		}

		return (*this);
	}

	template <typename _Ty, uInt size>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size> Vector<_Ty, size>::GetClamp(const _Ty& min, const _Ty& max) const noexcept {
		Vector<_Ty, size> vector = (*this);

		if NT_CONSTEXPR (size >= 2 && size <= 4) {
			vector.x = Clamp(this->x, min, max);
			vector.y = Clamp(this->y, min, max);

			if NT_CONSTEXPR(size >= 3)
				vector.z = Clamp(this->z, min, max);
			if NT_CONSTEXPR(size == 4)
				vector.w = Clamp(this->w, min, max);
		}
		else {
			for (uInt i = 0; i < size; ++i)
				vector.Array[i] = Clamp(vector.Array[i], min, max);
		}

		return vector;
	}


	template <typename _Ty, uInt size>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size> Vector<_Ty, size>::Copy() const noexcept {
		return (*this);
	}

	template <typename _Ty, uInt size>
	template <class _Op>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size>& Vector<_Ty, size>::Apply() noexcept {
		if NT_CONSTEXPR(size >= 2 && size <= 4) {
			this->x = _Op::Apply(this->x);
			this->y = _Op::Apply(this->y);

			if NT_CONSTEXPR(size >= 3) {
				this->z = _Op::Apply(this->z);
				if NT_CONSTEXPR(size == 4)
					this->w = _Op::Apply(this->w);
			}
		}
		else {
			for (uInt i = 0; i < Size; ++i)
				this->Array[i] = _Op::Apply(this->Array[i]);
		}

		return (*this);
	}

	template <typename _Ty, uInt size>
	template <class _Op, uInt index>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size>& Vector<_Ty, size>::ApplyImpl() noexcept {
		return (*this);
	}

	template <typename _Ty, uInt size>
	template <class _Op, uInt index, typename First, typename... Rest>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size>& Vector<_Ty, size>::ApplyImpl(First value, Rest... rest) noexcept {
		if NT_CONSTEXPR (index < size) {
			Set<index>(_Op::Apply(Get<index>(), static_cast<_Ty>(value)));
			return ApplyImpl<_Op, index + 1>(rest...);
		}
		else {
			return (*this);
		}
	}

	template <typename _Ty, uInt size>
	template <class _Op>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size>& Vector<_Ty, size>::Apply(const Vector& other) noexcept {
		if NT_CONSTEXPR (size >= 2 && size <= 4) {
			if NT_CONSTEXPR(size == 2)
				return ApplyImpl<_Op, 0>(other.x, other.y);
			else if NT_CONSTEXPR(size == 3)
				return ApplyImpl<_Op, 0>(other.x, other.y, other.z);
			else if NT_CONSTEXPR(size == 4)
				return ApplyImpl<_Op, 0>(other.x, other.y, other.z, other.w);
		}
		else {
			for (uInt i = 0; i < Size; ++i)
				this->Array[i] = _Op::Apply(this->Array[i], other.Array[i]);

			return (*this);
		}
	}

	template <typename _Ty, uInt size>
	template <class _Op>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size>& Vector<_Ty, size>::Apply(const _Ty& scalar) noexcept {
		if NT_CONSTEXPR (size >= 2 && size <= 4) {
			if NT_CONSTEXPR(size == 2)
				return ApplyImpl<_Op, 0>(scalar, scalar);
			else if NT_CONSTEXPR(size == 3)
				return ApplyImpl<_Op, 0>(scalar, scalar, scalar);
			else if NT_CONSTEXPR(size == 4)
				return ApplyImpl<_Op, 0>(scalar, scalar, scalar, scalar);
		}
		else {
			for (uInt i = 0; i < Size; ++i)
				this->Array[i] = _Op::Apply(this->Array[i], scalar);

			return (*this);
		}
	}


	template <typename _Ty, uInt size>
	template <class _Op>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size> Vector<_Ty, size>::ApplyCopy(const Vector& other) const noexcept {
		return Copy().Apply<_Op>(other);
	}

	template <typename _Ty, uInt size>
	template <class _Op>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size> Vector<_Ty, size>::ApplyCopy(const _Ty& scalar) const noexcept {
		return Copy().Apply<_Op>(scalar);
	}

	template <typename _Ty, uInt size>
	template <class _Op>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size> Vector<_Ty, size>::ApplyCopy() const noexcept {
		return Copy().Apply<_Op>();
	}
	
	template <typename _Ty, uInt size>
	template <class _Op, uInt index>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Bool Vector<_Ty, size>::CompareImpl() const noexcept {
		return true;
	}

	template <typename _Ty, uInt size>
	template <class _Op, uInt index, typename First, typename... Rest>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Bool Vector<_Ty, size>::CompareImpl(First value, Rest... rest) const noexcept {
		if NT_CONSTEXPR (index < size) {
			if (!_Op::Apply(Get<index>(), static_cast<_Ty>(value)))
				return false;

			return CompareImpl<_Op, index + 1>(rest...);
		}
		else {
			return true;
		}
	}

	template <typename _Ty, uInt size>
	template <class _Op>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Bool Vector<_Ty, size>::Compare(const Vector& other) const noexcept {
		if NT_CONSTEXPR (size >= 2 && size <= 4) {
			if NT_CONSTEXPR(size == 2)
				return CompareImpl<_Op, 0>(other.x, other.y);
			if NT_CONSTEXPR(size == 3)
				return CompareImpl<_Op, 0>(other.x, other.y, other.z);
			if NT_CONSTEXPR(size == 4)
				return CompareImpl<_Op, 0>(other.x, other.y, other.z, other.w);
		}
		else {
			for (uInt i = 0; i < Size; ++i) {
				if (!_Op::Apply(this->Array[i], other.Array[i]))
					return false;
			}

			return true;
		}
	}

	template <typename _Ty, uInt size>
	template <class _Op>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Bool Vector<_Ty, size>::Compare(const _Ty& scalar) const noexcept {
		if NT_CONSTEXPR(size >= 2 && size <= 4) {
			if NT_CONSTEXPR(size == 2)
				return CompareImpl<_Op, 0>(scalar, scalar);
			if NT_CONSTEXPR(size == 3)
				return CompareImpl<_Op, 0>(scalar, scalar, scalar);
			if NT_CONSTEXPR(size == 4)
				return CompareImpl<_Op, 0>(scalar, scalar, scalar, scalar);
		}
		else {
			for (uInt i = 0; i < Size; ++i) {
				if (!_Op::Apply(this->Array[i], scalar))
					return false;
			}

			return true;
		}
	}

	template <typename _Ty, uInt size>
	constexpr Bool Vector<_Ty, size>::CompletelyNotEqual(const Vector& other) const noexcept {
		return Compare<BinOpNeq<_Ty>>(other);
	}

	template <typename _Ty, uInt size>
	template <class _Op>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Bool Vector<_Ty, size>::Compare() const noexcept {
		if NT_CONSTEXPR(size >= 2 && size <= 4) {
			if (!(_Op::Apply(this->x) && _Op::Apply(this->y)))
				return false;

			if NT_CONSTEXPR(size >= 3) {
				if NT_CONSTEXPR(!_Op::Apply(this->z))
					return false;
			}

			if NT_CONSTEXPR(size == 4) {
				if NT_CONSTEXPR (!_Op::Apply(this->w))
					return false;
			}
		}
		else {
			for (uInt i = 0; i < Size; ++i) {
				if (!_Op::Apply(this->Array[i]))
					return false;
			}
		}

		return true;
	}

	template <typename _Ty, uInt size>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size> Vector<_Ty, size>::Round() const noexcept {
		return ApplyCopy<UnOpRound<_Ty>>();
	}
	template <typename _Ty, uInt size>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size> Vector<_Ty, size>::Ceil() const noexcept {
		return ApplyCopy<UnOpCeil<_Ty>>();
	}
	template <typename _Ty, uInt size>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size> Vector<_Ty, size>::Floor() const noexcept {
		return ApplyCopy<UnOpFloor<_Ty>>();
	}
	template <typename _Ty, uInt size>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size> Vector<_Ty, size>::Trunc() const noexcept {
		return ApplyCopy<UnOpTrunc<_Ty>>();
	}
	template <typename _Ty, uInt size>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size> Vector<_Ty, size>::Abs() const noexcept {
		return ApplyCopy<UnOpAbs<_Ty>>();
	}
	template <typename _Ty, uInt size>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size> Vector<_Ty, size>::Sqrt() const noexcept {
		return ApplyCopy<UnOpSqrt<_Ty>>();
	}

	template <typename _Ty, uInt size>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size> Vector<_Ty, size>::Fract() const noexcept {
		return ApplyCopy<UnOpFract<_Ty>>();
	}
	template <typename _Ty, uInt size>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size> Vector<_Ty, size>::Permute() const noexcept {
		return ApplyCopy<UnOpPermute<_Ty>>();
	}
	template <typename _Ty, uInt size>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size> Vector<_Ty, size>::Fade() const noexcept {
		return ApplyCopy<UnOpFade<_Ty>>();
	}
	template <typename _Ty, uInt size>
	NT_FORCE_INLINE NT_CONSTEXPR 
	Vector<_Ty, size> Vector<_Ty, size>::Lerp(const Vector& a, const Vector& b) const noexcept {
		Vector result;

		if NT_CONSTEXPR (size >= 2 && size <= 4) {
			result.x = Nt::Lerp(a.x, b.x, this->x);
			result.y = Nt::Lerp(a.y, b.y, this->y);

			if NT_CONSTEXPR (size >= 3)
				result.z = Nt::Lerp(a.z, b.z, this->z);
			if NT_CONSTEXPR (size == 4)
				result.w = Nt::Lerp(a.w, b.w, this->w);
		}
		else {
			for (uInt i = 0; i < size; ++i)
				result.Array[i] = Nt::Lerp(a.Array[i], b.Array[i], this->Array[i]);
		}

		return result;
	}

	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size> Vector<_Ty, size>::Min(const Vector& other) const noexcept {
		return ApplyCopy<BinOpMin<_Ty>>(other);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size> Vector<_Ty, size>::Max(const Vector& other) const noexcept {
		return ApplyCopy<BinOpMax<_Ty>>(other);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size> Vector<_Ty, size>::Min(const _Ty& scalar) const noexcept {
		return ApplyCopy<BinOpMin<_Ty>>(scalar);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size> Vector<_Ty, size>::Max(const _Ty& scalar) const noexcept {
		return ApplyCopy<BinOpMax<_Ty>>(scalar);
	}


	// ============================================================================
	//		Operators with vector
	// ----------------------------------------------------------------------------
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size> Vector<_Ty, size>::operator + (const Vector& vector) const noexcept {
		return ApplyCopy<BinOpAdd<_Ty>>(vector);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size> Vector<_Ty, size>::operator - (const Vector& vector) const noexcept {
		return ApplyCopy<BinOpSub<_Ty>>(vector);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size> Vector<_Ty, size>::operator * (const Vector& vector) const noexcept {
		return ApplyCopy<BinOpMul<_Ty>>(vector);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size> Vector<_Ty, size>::operator / (const Vector& vector) const noexcept {
		return ApplyCopy<BinOpDiv<_Ty>>(vector);
	}
	template <typename _Ty, uInt size>
	template <Requires(std::is_integral_v<_Ty>)>
	NT_CONSTEXPR Vector<_Ty, size> Vector<_Ty, size>::operator % (const Vector& vector) const noexcept {
		return ApplyCopy<BinOpMod<_Ty>>(vector);
	}

	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size>& Vector<_Ty, size>::operator += (const Vector& vector) noexcept {
		return Apply<BinOpAdd<_Ty>>(vector);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size>& Vector<_Ty, size>::operator -= (const Vector& vector) noexcept {
		return Apply<BinOpSub<_Ty>>(vector);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size>& Vector<_Ty, size>::operator *= (const Vector& vector) noexcept {
		return Apply<BinOpMul<_Ty>>(vector);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size>& Vector<_Ty, size>::operator /= (const Vector& vector) noexcept {
		return Apply<BinOpDiv<_Ty>>(vector);
	}
	template <typename _Ty, uInt size>
	template <Requires(std::is_integral_v<_Ty>)>
	NT_CONSTEXPR Vector<_Ty, size>& Vector<_Ty, size>::operator %= (const Vector& vector) noexcept {
		return Apply<BinOpMod<_Ty>>(vector);
	}

	template <typename _Ty, uInt size>
	NT_CONSTEXPR Bool Vector<_Ty, size>::operator > (const Vector& vector) const noexcept {
		return Compare<BinOpGt<_Ty>>(vector);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Bool Vector<_Ty, size>::operator < (const Vector& vector) const noexcept {
		return Compare<BinOpLt<_Ty>>(vector);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Bool Vector<_Ty, size>::operator >= (const Vector& vector) const noexcept {
		return Compare<BinOpGte<_Ty>>(vector);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Bool Vector<_Ty, size>::operator <= (const Vector& vector) const noexcept {
		return Compare<BinOpLte<_Ty>>(vector);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Bool Vector<_Ty, size>::operator == (const Vector& vector) const noexcept {
		return Compare<BinOpEq<_Ty>>(vector);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Bool Vector<_Ty, size>::operator != (const Vector& vector) const noexcept {
		return !Compare<BinOpEq<_Ty>>(vector);
	}

	// ============================================================================
	//		Operators with scalars
	// ----------------------------------------------------------------------------
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size> Vector<_Ty, size>::operator + (const _Ty& scalar) const noexcept {
		return ApplyCopy<BinOpAdd<_Ty>>(scalar);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size> Vector<_Ty, size>::operator - (const _Ty& scalar) const noexcept {
		return ApplyCopy<BinOpSub<_Ty>>(scalar);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size> Vector<_Ty, size>::operator * (const _Ty& scalar) const noexcept {
		return ApplyCopy<BinOpMul<_Ty>>(scalar);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size> Vector<_Ty, size>::operator / (const _Ty& scalar) const noexcept {
		return ApplyCopy<BinOpDiv<_Ty>>(scalar);
	}
	template <typename _Ty, uInt size>
	template <Requires(std::is_integral_v<_Ty>)>
	NT_CONSTEXPR Vector<_Ty, size> Vector<_Ty, size>::operator % (const _Ty& scalar) const noexcept {
		return ApplyCopy<BinOpMod<_Ty>>(scalar);
	}

	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size>& Vector<_Ty, size>::operator += (const _Ty& scalar) noexcept {
		return Apply<BinOpAdd<_Ty>>(scalar);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size>& Vector<_Ty, size>::operator -= (const _Ty& scalar) noexcept {
		return Apply<BinOpSub<_Ty>>(scalar);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size>& Vector<_Ty, size>::operator *= (const _Ty& scalar) noexcept {
		return Apply<BinOpMul<_Ty>>(scalar);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Vector<_Ty, size>& Vector<_Ty, size>::operator /= (const _Ty& scalar) noexcept {
		return Apply<BinOpDiv<_Ty>>(scalar);
	}
	template <typename _Ty, uInt size>
	template <Requires(std::is_integral_v<_Ty>)>
	NT_CONSTEXPR Vector<_Ty, size>& Vector<_Ty, size>::operator %= (const _Ty& scalar) noexcept {
		return Apply<BinOpMod<_Ty>>(scalar);
	}

	template <typename _Ty, uInt size>
	NT_CONSTEXPR Bool Vector<_Ty, size>::operator > (const _Ty& scalar) const noexcept {
		return Compare<BinOpGt<_Ty>>(scalar);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Bool Vector<_Ty, size>::operator < (const _Ty& scalar) const noexcept {
		return Compare<BinOpLt<_Ty>>(scalar);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Bool Vector<_Ty, size>::operator >= (const _Ty& scalar) const noexcept {
		return Compare<BinOpGte<_Ty>>(scalar);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Bool Vector<_Ty, size>::operator <= (const _Ty& scalar) const noexcept {
		return Compare<BinOpLte<_Ty>>(scalar);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Bool Vector<_Ty, size>::operator == (const _Ty& scalar) const noexcept {
		return Compare<BinOpEq<_Ty>>(scalar);
	}
	template <typename _Ty, uInt size>
	NT_CONSTEXPR Bool Vector<_Ty, size>::operator != (const _Ty& scalar) const noexcept {
		return Compare<BinOpNeq<_Ty>>(scalar);
	}

// ============================================================================
//		Others operators
// ----------------------------------------------------------------------------
	template <typename _Ty, uInt size>
	NT_CONSTEXPR const _Ty& Vector<_Ty, size>::operator [] (const uInt& index) const {
		Assert(index < size, "Out of range");

		if NT_CONSTEXPR (size >= 2 && size <= 4) {
			if (index == 0)
				return this->x;
			if (index == 1)
				return this->y;

			if NT_CONSTEXPR(size >= 3) {
				if (index == 2)
					return this->z;
			}

			if NT_CONSTEXPR(size == 4) {
				if (index == 3)
					return this->w;
			}
		}
		else {
			return this->Array[index];
		}
	}

	template <typename _Ty, uInt size>
	NT_CONSTEXPR _Ty& Vector<_Ty, size>::operator [] (const uInt& index) {
		Assert(index < size, "Out of range");

		if NT_CONSTEXPR(size >= 2 && size <= 4) {
			if (index == 0)
				return this->x;
			if (index == 1)
				return this->y;

			if NT_CONSTEXPR(size >= 3) {
				if (index == 2)
					return this->z;
			}

			if NT_CONSTEXPR(size == 4) {
				if (index == 3)
					return this->w;
			}
		}
		else {
			return this->Array[index];
		}
	}

	template <typename _Ty, uInt size>
	NT_FORCE_INLINE NT_CONSTEXPR Vector<_Ty, size> Vector<_Ty, size>::operator + () const noexcept {
		return (*this);
	}

	template <typename _Ty, uInt size>
	NT_FORCE_INLINE NT_CONSTEXPR Vector<_Ty, size> Vector<_Ty, size>::operator - () const noexcept {
		return ApplyCopy<UnOpNegate<_Ty>>();
	}

	template <typename _Ty, uInt size>
	NT_FORCE_INLINE NT_CONSTEXPR Bool Vector<_Ty, size>::operator ! () const noexcept {
		return Compare<UnOpLogicalNot<_Ty>>();
	}

	template <typename _Ty, uInt size>
	NT_FORCE_INLINE NT_CONSTEXPR Vector<_Ty, size>& Vector<_Ty, size>::operator = (const Vector<_Ty, size>& vector) noexcept {
		if NT_CONSTEXPR(size >= 2 && size <= 4) {
			this->x = vector.x;
			this->y = vector.y;

			if NT_CONSTEXPR(size >= 3)
				this->z = vector.z;
			if NT_CONSTEXPR(size == 4)
				this->w = vector.w;
		}
		else {
			for (uInt i = 0; i < size; ++i)
				this->Array[i] = vector.Array[i];
		}

		return (*this);
	}
}