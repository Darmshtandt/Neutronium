#pragma once

#include <Nt/Core/NtError.h>

#include <memory>

template <class _Ty>
NT_CONSTEXPR const _Ty& RequireNotNull(const _Ty& pointer, const std::string_view& message) {
#ifdef _DEBUG
	if (pointer == nullptr)
		Raise(message);
#endif
	return pointer;
}

template <class _Ty>
NT_CONSTEXPR _Ty& RequireNotNull(_Ty& pointer, const std::string_view& message) {
#ifdef _DEBUG
	if (pointer == nullptr)
		Raise(message);
#endif
	return pointer;
}

template <class _Ty>
NT_CONSTEXPR const _Ty& RequireNotNull(const _Ty& pointer) {
#ifdef _DEBUG
	if (pointer == nullptr)
		Raise(std::string(typeid(std::remove_cvref_t<_Ty>).name()) + " is null");
#endif
	return pointer;
}

template <class _Ty>
NT_CONSTEXPR _Ty& RequireNotNull(_Ty& pointer) {
#ifdef _DEBUG
	if (pointer == nullptr)
		Raise(std::string(typeid(std::remove_cvref_t<_Ty>).name()) + " is null");
#endif
	return pointer;
}

#ifdef _DEBUG
template <class _Ty>
class BaseNotNull {
public:
	BaseNotNull(_Ty pointer) :
		m_Pointer(RequireNotNull(pointer)) {
	}

	template <class _U>
	NT_NODISCARD _U DynamicCast() const {
		return RequireNotNull(dynamic_cast<_U>(m_Pointer));
	}

	template <class _U>
	NT_NODISCARD _U ReinterpretCast() const {
		return RequireNotNull(reinterpret_cast<_U>(m_Pointer));
	}

	BaseNotNull& operator = (_Ty& pointer) {
		if (m_Pointer != pointer)
			m_Pointer = RequireNotNull(pointer);
		return (*this);
	}
	//NotNull& operator = (_Ty&& pointer) {
	//	if (m_Pointer != pointer) {
	//		m_Pointer = RequireNotNull(pointer);
	//		pointer = nullptr;
	//	}
	//	return (*this);
	//}

	const _Ty& operator -> () const noexcept {
		return m_Pointer;
	}
	_Ty& operator -> () noexcept {
		return m_Pointer;
	}

	NT_NODISCARD const std::remove_pointer_t<_Ty>& operator * () const {
		return *m_Pointer;
	}
	NT_NODISCARD std::remove_pointer_t<_Ty>& operator * () {
		return *m_Pointer;
	}

	NT_NODISCARD operator _Ty () const {
		return m_Pointer;
	}
	NT_NODISCARD operator _Ty () {
		return m_Pointer;
	}

	NT_NODISCARD const _Ty& Get() const noexcept {
		return m_Pointer;
	}
	NT_NODISCARD _Ty& Get() noexcept {
		return m_Pointer;
	}

private:
	_Ty m_Pointer;
};

template <typename _Ty>
struct is_smart_pointer : std::false_type
{
};

template <typename _Ty>
struct is_smart_pointer<std::shared_ptr<_Ty>> : std::true_type
{
};

template <typename _Ty>
struct is_smart_pointer<std::unique_ptr<_Ty>> : std::true_type
{
};

template <typename _Ty>
struct is_smart_pointer<std::weak_ptr<_Ty>> : std::true_type
{
};

template <typename _Ty>
NT_CONSTEXPR Bool is_smart_pointer_v = is_smart_pointer<_Ty>::value;

template <typename _Ty, typename _Enable = void>
class NotNull;

template <class _Ty>
class NotNull<_Ty, std::enable_if_t<std::is_pointer_v<_Ty>>> : public BaseNotNull<_Ty> {
public:
	NotNull(_Ty pointer) :
		BaseNotNull<_Ty>(pointer)
	{
	}
};

template <class _Ty>
class NotNull<_Ty, std::enable_if_t<is_smart_pointer_v<_Ty>>> : public BaseNotNull<_Ty&> {
public:
	NotNull(_Ty& pointer) :
		BaseNotNull<_Ty&>(pointer)
	{
	}
};

template <class _Ty>
class NotNull<std::weak_ptr<_Ty>> {
	static_assert(sizeof(_Ty) == 0,
		"NotNull<std::weak_ptr<T>> is ill-formed. Use .lock() and check validity instead.");
};
#else
template <class _Ty>
using NotNull = _Ty;
#endif