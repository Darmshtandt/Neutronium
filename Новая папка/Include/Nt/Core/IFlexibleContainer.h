#pragma once

namespace Nt {
	template <class _Ty>
	struct IFlexibleContainer {
		enum class Behavior : unsigned {
			NONE = 0x0000,
			PROHIBITE_ADDING = 0x0001,
			PROHIBITE_REMOVING = 0x0002,
			PROHIBITE_ADDING_ON_OVERFLOW = 0x0004,
			PROHIBITE_REMOVING_ON_OVERFLOW = 0x0008,
			PROHIBITE_ADDING_DUPLICATES = 0x00010,

			// Allow removal of last element on overflow at adding a new element near the end
			ALLOW_REMOVE_LAST_AT_ADDING_NEAR_END_ON_OVERFLOW = 0x0020,

			// Allow removal of first element on overflow at adding a new element near the end
			ALLOW_REMOVE_FIRST_AT_ADDING_NEAR_END_ON_OVERFLOW = 0x0040,

			// Allow removal of last element on overflow at adding a new element near the start
			ALLOW_REMOVE_LAST_AT_ADDING_NEAR_START_ON_OVERFLOW = 0x0080,

			// Allow removal of first element on overflow at adding a new element near the start
			ALLOW_REMOVE_FIRST_AT_ADDING_NEAR_START_ON_OVERFLOW = 0x0100,

			// Allow to reserve memory when setting the minimum limit
			ALLOW_RESERVE_WHEN_SETTING_MINIMUM_LIMIT = 0x0200,

			// Allow to compress list when setting maximum limit
			ALLOW_COMPRESS_LIST_WHEN_SETTING_MAXIMUM_LIMIT = 0x0400,

			// Allow deletion of items when setting max limit
			ALLOW_DELETION_ELEMENTS_WHEN_SETTING_MAXIMUM_LIMIT = 0x0800
		};

		static const uInt Start = 0;
		static const uInt End = uInt(-1);

	public:
		virtual ~IFlexibleContainer() = 0;

		virtual void Assign(const IFlexibleContainer<_Ty>& elements) = 0;
		virtual void Assign(const std::initializer_list<_Ty>& elements) = 0;

#ifdef _LIST_
		virtual void Assign(const std::list<_Ty>& elements) = 0;
#endif

#ifdef _VECTOR_
		virtual void Assign(const std::vector<_Ty>& elements) = 0;
#endif

#ifdef _STACK_
		virtual void Assign(const std::stack<_Ty>& elements) = 0;
#endif

#ifdef _SET_
		virtual void Assign(const std::set<_Ty>& elements) = 0;
#endif

#ifdef _UNORDERED_SET_
		virtual void Assign(const std::unordered_set<_Ty>& elements) = 0;
#endif

		virtual void SetSize(const uInt& size) = 0;
		virtual void Expand(const uInt& value) = 0;
		virtual void Decrease(const uInt& value) = 0;
		virtual void FreeUnallocatedSpace() = 0;
		virtual void Defragmentation() = 0;

		// The Compress method removes empty cells from the end 
		// until they run out, or until the size reaches sizeAfterCompression
		//
		// Return value:
		// Number of cells that weren't deleted because they weren't empty
		virtual uInt Compress(const uInt& sizeAfterCompression) = 0;

		virtual void Add(const _Ty& element) = 0;
		virtual void AddRange(const IFlexibleContainer<_Ty>& elements) = 0;
		virtual void Insert(const uInt& index, const _Ty& element) = 0;

		virtual void Remove(const uInt& index) = 0;
		virtual void RemoveRange(const uInt& start, const uInt& end) = 0;
		virtual void RemoveSome(const IFlexibleContainer<uInt>& indices) = 0;
		virtual void RemoveDuplicate() = 0;
		virtual void Clear() = 0;

		virtual void Swap(const uInt& index_1, const uInt& index_2) = 0;
		virtual void Change(const uInt& index, const _Ty& newElement) = 0;
		virtual void Inverse() = 0;

		virtual void SaveReservedCopy() = 0;
		virtual void LoadReservedCopy() = 0;

		void AddBehavior(const Behavior& behavior) {
			m_Behavior |= behavior;
		}
		void RemoveBehavior(const Behavior& behavior) {
			m_Behavior &= ~behavior;
		}
		void ClearBehavior() {
			m_Behavior = Behavior::NONE;
		}

		void EnableStrict() noexcept final {
			m_IsStrickEnabled = true;
		}
		void DisableStrict() noexcept final {
			m_IsStrickEnabled = false;
		}

		virtual uInt Find(const _Ty& element) = 0;
		virtual IFlexibleContainer<uInt> FindSome(const IFlexibleContainer<_Ty>& elements) = 0;
		virtual IFlexibleContainer<_Ty> FindRange(const IFlexibleContainer<_Ty>& elements) = 0;

		virtual IFlexibleContainer<_Ty> Copy() = 0;
		virtual IFlexibleContainer<_Ty> CopyRange() = 0;

		virtual IFlexibleContainer<_Ty> GetRange(const uInt& end) = 0;
		virtual IFlexibleContainer<_Ty> GetRange(const uInt& start, const uInt& end) = 0;


#ifdef _LIST_
		virtual std::list<_Ty> ToStdList() = 0;
#endif
#ifdef _VECTOR_
		virtual std::vector<_Ty> ToVector() = 0;
#endif
#ifdef _STACK_
		virtual std::stack<_Ty> ToStack() = 0;
#endif
#ifdef _SET_
		virtual std::set<_Ty> ToSet() = 0;
#endif
#ifdef _UNORDERED_SET_
		virtual std::unordered_set<_Ty> ToUnorderedSet() = 0;
#endif

		_Ty& operator == (const IFlexibleContainer<_Ty>& container) const noexcept {
			for (uInt i = 0; i < m_Size; ++i)
				if ((*this)[i] != container[i])
					return false;
			return true;
		}
		virtual _Ty& operator [] (const uInt& index) const = 0;
		_Ty& operator [] (const uInt& index) {
			return (*this)[index];
		}

		uInt Size() const noexcept final {
			return m_Size;
		}
		Bool IsStrickEnabled() const noexcept final {
			return m_IsStrickEnabled;
		}

		uInt GetMinLimit() const noexcept final {
			return m_MinLimit;
		}
		uInt GetMaxLimit() const noexcept final {
			return m_MaxLimit;
		}
		Behavior GetBehavior() const noexcept final {
			return m_Behavior;
		}

		void SetMinimumLimit(const uInt& minLimit) final {
			m_MinLimit = minLimit;
			if (m_MinLimit >= m_Size) {
				if (m_Behavior & Behavior::ALLOW_RESERVE_WHEN_SETTING_MINIMUM_LIMIT)
					Expand(m_MinLimit - m_Size);
				else if (m_IsStrickEnabled)
					Raise("IFlexibleContainer: container size is less than the minimum limit.");
				else
					Log::Warning("IFlexibleContainer: container size is less than the minimum limit.");
			}
		}
		void SetMaximumLimit(const uInt& maxLimit) final {
			m_MaxLimit = maxLimit;
			if (m_MaxLimit < m_Size) {
				if (m_Behavior & Behavior::ALLOW_COMPRESS_LIST_WHEN_SETTING_MAXIMUM_LIMIT)
					if (Compress(m_MaxLimit) == 0)
						return;

				if (m_Behavior & Behavior::ALLOW_DELETION_ELEMENTS_WHEN_SETTING_MAXIMUM_LIMIT)
					RemoveRange(m_MaxLimit + 1, m_Size);
				else if (m_IsStrickEnabled)
					Raise("IFlexibleContainer: container size is less than the minimum limit.");
				else
					Log::Warning("IFlexibleContainer: container size is less than the minimum limit.");
			}
		}
		void SetBehavior(const Behavior& behavior) final {
			m_Behavior = behavior;
		}

	protected:
		uInt m_Size;
		uInt m_MinLimit;
		uInt m_MaxLimit;
		Bool m_IsStrickEnabled;
		Behavior m_Behavior;
	};
}