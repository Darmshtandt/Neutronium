#include <Nt/Core/Math/Vector.h>
#include <Nt/Core/Utilities.h>

namespace Nt {
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size>::Vector(const std::initializer_list<_Ty>& list) noexcept {
		if (list.size() == 0)
			return;

		uInt scalarIndex = 0;
		for (const _Ty& scalar : list) {
			this->Array[scalarIndex] = scalar;
			++scalarIndex;
		}

		for (; scalarIndex < size; ++scalarIndex)
			this->Array[scalarIndex] = _Ty(0);
	}

	template <typename _Ty, uInt size>
	template <uInt S, typename>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::GetCross(const Vector& vector) const noexcept {
		Vector result;
		result.Array[0] = (this->Array[1] * vector.Array[2] - this->Array[2] * vector.Array[1]);
		result.Array[1] = (this->Array[2] * vector.Array[0] - this->Array[0] * vector.Array[2]);
		result.Array[2] = (this->Array[0] * vector.Array[1] - this->Array[1] * vector.Array[0]);
		return result;
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size>& Vector<_Ty, size>::Fill(const _Ty& scalar) noexcept {
		this->Array.fill(scalar);
		return (*this);
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size>& Vector<_Ty, size>::Set(const uInt& index, const _Ty& scalar) {
		this->Array[index] = scalar;
		return (*this);
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 _Ty Vector<_Ty, size>::LengthSquare() const noexcept {
		_Ty lengthSquare = 0;
		for (uInt i = 0; i < Size; ++i)
			lengthSquare += this->Array[i] * this->Array[i];

		return lengthSquare;
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 _Ty Vector<_Ty, size>::Length() const noexcept {
		return _Ty(sqrt((Double)LengthSquare()));
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 _Ty Vector<_Ty, size>::GetDistance(const Vector& vector) const noexcept {
		return (operator - (vector)).Length();
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 _Ty Vector<_Ty, size>::GetSquareDistance(const Vector& vector) const noexcept {
		return (operator - (vector)).LengthSquare();
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::GetNormalize() const noexcept {
		return operator / (Length());
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 _Ty Vector<_Ty, size>::Dot(const Vector& vector) const noexcept {
		_Ty dot = 0;
		for (uInt i = 0; i < Size; ++i)
			dot += this->Array[i] * vector.Array[i];

		return dot;
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 String Vector<_Ty, size>::ToString() const noexcept {
		_CONSTEXPR20 uInt literalCount = ('z' - 'a');

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
	_CONSTEXPR20 void Vector<_Ty, size>::FromString(const String& string) const noexcept {
		const uInt startIndex = string.find("{");
		const uInt endIndex = string.find("};");

		if (startIndex == uInt(-1) || endIndex == uInt(-1))
			return;

		String vectorString = string.substr(startIndex + 1, endIndex - 2);
		vectorString.erase(std::remove_if(vectorString.begin(), vectorString.end(),
			[] (const Char& symbol) {
				return !(std::isalpha(symbol) || symbol == ',');
			}), vectorString.end());

		_CONSTEXPR20 uInt literalCount = ('z' - 'a');

		const std::vector<String> allCoords = string.Split(",");
		for (const String& coord : allCoords) {
			if (coord.empty())
				continue;

			const std::vector<String> data = coord.Split(":");
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
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::GetClamp(const _Ty& min, const _Ty& max) const noexcept {
		Vector<_Ty, size> vector = (*this);

		for (uInt i = 0; i < size; ++i) {
			if (vector.Array[i] < min)
				vector.Array[i] = min;
			else if (vector.Array[i] > max)
				vector.Array[i] = max;
		}

		return vector;
	}


	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::Copy() const noexcept {
		return (*this);
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size>& Vector<_Ty, size>::Apply(const Vector& other, auto operation) noexcept {
		for (uInt i = 0; i < Size; ++i)
			this->Array[i] = operation(this->Array[i], other.Array[i]);

		return (*this);
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size>& Vector<_Ty, size>::Apply(const _Ty& scalar, auto operation) noexcept {
		for (uInt i = 0; i < Size; ++i)
			this->Array[i] = operation(this->Array[i], scalar);

		return (*this);
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::ApplyCopy(const Vector& other, auto operation) const noexcept {
		Vector vector;
		for (uInt i = 0; i < Size; ++i)
			vector.Array[i] = operation(this->Array[i], other.Array[i]);

		return vector;
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::ApplyCopy(const _Ty& scalar, auto operation) const noexcept {
		Vector vector;
		for (uInt i = 0; i < Size; ++i)
			vector.Array[i] = operation(this->Array[i], scalar);

		return vector;
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::ApplyCopy(auto operation) const noexcept {
		Vector vector;
		for (uInt i = 0; i < Size; ++i)
			vector.Array[i] = operation(this->Array[i]);

		return vector;
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 Bool Vector<_Ty, size>::Compare(const Vector& vector, auto comparator) const noexcept {
		for (uInt i = 0; i < Size; ++i) {
			if (!comparator(this->Array[i], vector.Array[i]))
				return false;
		}

		return true;
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Bool Vector<_Ty, size>::Compare(const _Ty& scalar, auto comparator) const noexcept {
		for (uInt i = 0; i < Size; ++i) {
			if (!comparator(this->Array[i], scalar))
				return false;
		}

		return true;
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::Round() const noexcept {
		return ApplyCopy([](auto scalar) { return (_Ty)std::round((Double)scalar); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::Ceil() const noexcept {
		return ApplyCopy([](auto scalar) { return (_Ty)std::ceil((Double)scalar); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::Floor() const noexcept {
		return ApplyCopy([](auto scalar) { return (_Ty)std::floor((Double)scalar); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::Trunc() const noexcept {
		return ApplyCopy([](auto scalar) { return (_Ty)std::trunc((Double)scalar); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::Abs() const noexcept {
		return ApplyCopy([](auto scalar) { return (_Ty)std::abs(scalar); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::Sqrt() const noexcept {
		return ApplyCopy([](auto scalar) { return (_Ty)std::sqrt(scalar); });
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::Fract() const noexcept {
		return ApplyCopy([](auto scalar) { return Nt::Fract(scalar); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::Permute() const noexcept {
		return ApplyCopy([](auto scalar) { return Nt::Permute(scalar); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::Fade() const noexcept {
		return ApplyCopy([](auto scalar) { return Nt::Fade(scalar); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::Lerp(const Vector& a, const Vector& b) const noexcept {
		Vector result = { };
		for (uInt i = 0; i < size; ++i)
			result.Array[i] = Nt::Lerp(a.Array[i], b.Array[i], this->Array[i]);

		return result;
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::Min(const Vector& other) const noexcept {
		return ApplyCopy(other, [](auto left, auto right) { return (left < right) ? left : right; });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::Max(const Vector& other) const noexcept {
		return ApplyCopy(other, [](auto left, auto right) { return (left > right) ? left : right; });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::Min(const _Ty& scalar) const noexcept {
		return ApplyCopy(scalar, [](auto left, auto right) { return (left < right) ? left : right; });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::Max(const _Ty& scalar) const noexcept {
		return ApplyCopy(scalar, [](auto left, auto right) { return (left > right) ? left : right; });
	}


	// ============================================================================
	//		Operators with vector
	// ----------------------------------------------------------------------------
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator + (const Vector& vector) const noexcept {
		return ApplyCopy(vector, [](auto left, auto right) { return (left + right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator - (const Vector& vector) const noexcept {
		return ApplyCopy(vector, [](auto left, auto right) { return (left - right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator * (const Vector& vector) const noexcept {
		return ApplyCopy(vector, [](auto left, auto right) { return (left * right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator / (const Vector& vector) const noexcept {
		return ApplyCopy(vector, [](auto left, auto right) { return (left / right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator % (const Vector& vector) const noexcept {
		return ApplyCopy(vector, [](auto left, auto right) { return (left % right); });
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator += (const Vector& vector) noexcept {
		return Apply(vector, [](auto left, auto right) { return (left + right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator -= (const Vector& vector) noexcept {
		return Apply(vector, [](auto left, auto right) { return (left - right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator *= (const Vector& vector) noexcept {
		return Apply(vector, [](auto left, auto right) { return (left * right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator /= (const Vector& vector) noexcept {
		return Apply(vector, [](auto left, auto right) { return (left / right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator %= (const Vector& vector) noexcept {
		return Apply(vector, [](auto left, auto right) { return (left % right); });
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 Bool Vector<_Ty, size>::operator > (const Vector& vector) const noexcept {
		return Compare(vector, [](auto left, auto right) { return (left > right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Bool Vector<_Ty, size>::operator < (const Vector& vector) const noexcept {
		return Compare(vector, [](auto left, auto right) { return (left < right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Bool Vector<_Ty, size>::operator >= (const Vector& vector) const noexcept {
		return Compare(vector, [](auto left, auto right) { return (left >= right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Bool Vector<_Ty, size>::operator <= (const Vector& vector) const noexcept {
		return Compare(vector, [](auto left, auto right) { return (left <= right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Bool Vector<_Ty, size>::operator == (const Vector& vector) const noexcept {
		return Compare(vector, [](auto left, auto right) { return (left == right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Bool Vector<_Ty, size>::operator != (const Vector& vector) const noexcept {
		return (!operator == (vector));
	}

	// ============================================================================
	//		Operators with scalars
	// ----------------------------------------------------------------------------
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator + (const _Ty& scalar) const noexcept {
		return ApplyCopy(scalar, [](auto left, auto right) { return (left + right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator - (const _Ty& scalar) const noexcept {
		return ApplyCopy(scalar, [](auto left, auto right) { return (left - right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator * (const _Ty& scalar) const noexcept {
		return ApplyCopy(scalar, [](auto left, auto right) { return (left * right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator / (const _Ty& scalar) const noexcept {
		return ApplyCopy(scalar, [](auto left, auto right) { return (left / right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator % (const _Ty& scalar) const noexcept {
		return ApplyCopy(scalar, [](auto left, auto right) { return (left % right); });
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator += (const _Ty& scalar) noexcept {
		return Apply(scalar, [](auto left, auto right) { return (left + right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator -= (const _Ty& scalar) noexcept {
		return Apply(scalar, [](auto left, auto right) { return (left - right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator *= (const _Ty& scalar) noexcept {
		return Apply(scalar, [](auto left, auto right) { return (left * right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator /= (const _Ty& scalar) noexcept {
		return Apply(scalar, [](auto left, auto right) { return (left / right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator %= (const _Ty& scalar) noexcept {
		return Apply(scalar, [](auto left, auto right) { return (left % right); });
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 Bool Vector<_Ty, size>::operator > (const _Ty& scalar) const noexcept {
		return Compare(scalar, [](auto left, auto right) { return (left > right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Bool Vector<_Ty, size>::operator < (const _Ty& scalar) const noexcept {
		return Compare(scalar, [](auto left, auto right) { return (left < right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Bool Vector<_Ty, size>::operator >= (const _Ty& scalar) const noexcept {
		return Compare(scalar, [](auto left, auto right) { return (left >= right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Bool Vector<_Ty, size>::operator <= (const _Ty& scalar) const noexcept {
		return Compare(scalar, [](auto left, auto right) { return (left <= right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Bool Vector<_Ty, size>::operator == (const _Ty& scalar) const noexcept {
		return Compare(scalar, [](auto left, auto right) { return (left == right); });
	}
	template <typename _Ty, uInt size>
	_CONSTEXPR20 Bool Vector<_Ty, size>::operator != (const _Ty& scalar) const noexcept {
		return Compare(scalar, [](auto left, auto right) { return (left != right); });
	}

// ============================================================================
//		Others operators
// ----------------------------------------------------------------------------

	template <typename _Ty, uInt size>
	_CONSTEXPR20 const _Ty& Vector<_Ty, size>::operator [] (const uInt& index) const {
		if (index >= Size) {
			Raise("Index out of range");
			return 0;
		}

		return this->Array[index];
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 _Ty& Vector<_Ty, size>::operator [] (const uInt& index) {
		if (index >= Size)
			Raise("Index out of range");

		return this->Array[index];
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator + () const noexcept {
		return (*this);
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size> Vector<_Ty, size>::operator - () const noexcept {
		return operator * ((_Ty)-1);
	}

	template <typename _Ty, uInt size>
	_CONSTEXPR20 Vector<_Ty, size>& Vector<_Ty, size>::operator = (const Vector<_Ty, size>& vector) {
		for (uInt i = 0; i < size; ++i)
			this->Array[i] = vector.Array[i];

		return (*this);
	}
}