#pragma once

namespace Nt {
//	template <typename _Ty1, typename _Ty2>
//	inline constexpr Bool Is_seguence_container = std::_Is_any_of_v < _Ty1,
//#ifdef _LIST_
//		std::list<_Ty2>,
//#endif
//#ifdef _FORWARD_LIST_
//		std::forward_list<_Ty2>,
//#endif
//#ifdef _VECTOR_
//		std::vector<_Ty2>,
//#endif
//#ifdef _DEQUE_
//		std::deque<_Ty2>,
//#endif
//	>;
//
//	template <typename _Ty1, typename _Ty2>
//	inline constexpr Bool Is_container_adaptors = std::_Is_any_of_v<_Ty1,
//#ifdef _STACK_
//		std::stack<_Ty2>,
//#endif
//#ifdef _QUEUE_
//		std::queue<_Ty2>,
//		std::priority_queue<_Ty2>,
//#endif
//	>;
//
//	template <typename _Ty1, typename _Ty2>
//	inline constexpr Bool Is_associative_container = std::_Is_any_of_v<_Ty1,
//#ifdef _SET_
//		std::set<_Ty2>,
//		std::multiset<_Ty2>,
//#endif
//	>;
//
//	template <typename _Ty1, typename _Ty2>
//	inline constexpr Bool Is_unordered_container = std::_Is_any_of_v<_Ty1,
//#ifdef _UNORDERED_SET_
//		std::unordered_set<_Ty2>
//		std::unordered_multiset<_Ty2>
//#endif
//	>;

	template <class _Ty>
	class FlexibleList {
	private:
		struct Node {
			operator _Ty* () const noexcept {
				return pData;
			}

			Node* pPrev = nullptr;
			Node* pNext = nullptr;
			std::shared_ptr<_Ty> pData = nullptr;
		};

	public:
		enum class Behavior : unsigned {
			NONE,
			PROHIBITE_ADDING,
			PROHIBITE_REMOVING,
			PROHIBITE_ADDING_ON_OVERFLOW,
			PROHIBITE_REMOVING_ON_OVERFLOW,
			PROHIBITE_ADDING_DUPLICATES,

			// Allow removal of last element on overflow at adding a new element near the end
			ALLOW_REMOVE_LAST_AT_ADDING_NEAR_END_ON_OVERFLOW,

			// Allow removal of first element on overflow at adding a new element near the end
			ALLOW_REMOVE_FIRST_AT_ADDING_NEAR_END_ON_OVERFLOW,

			// Allow removal of last element on overflow at adding a new element near the start
			ALLOW_REMOVE_LAST_AT_ADDING_NEAR_START_ON_OVERFLOW,

			// Allow removal of first element on overflow at adding a new element near the start
			ALLOW_REMOVE_FIRST_AT_ADDING_NEAR_START_ON_OVERFLOW,

			// Allow to reserve memory when setting the minimum limit
			ALLOW_RESERVE_WHEN_SETTING_MINIMUM_LIMIT,

			// Allow to compress list when setting maximum limit
			ALLOW_COMPRESS_LIST_WHEN_SETTING_MAXIMUM_LIMIT,

			// Allow deletion of items when setting max limit
			ALLOW_DELETION_ELEMENTS_WHEN_SETTING_MAXIMUM_LIMIT
		};

		static const uInt Start = 0;
		static const uInt End = uInt(-1);

	public:
		FlexibleList() = default;
		FlexibleList(const FlexibleList& elements) {
			Assign(elements);
		}
		FlexibleList(const std::initializer_list<_Ty>& elements) {
			Assign(elements);
		}

		void SetSize(const uInt& size);
		void Expand(const uInt& value) {
			if (value == 0) {
				Log::Warning("Expand(): value is 0");
				return;
			}

			for (uInt i = 0; i < value; ++i) {
				
			}
		}
		void Decrease(const uInt& value);
		void Defragmentation();
		void FreeUnallocatedSpace();

		void Add(const _Ty& element) {
			if (m_pFirst == nullptr) {
				if (m_Size > 0)
					Raise("The FlexibleList is corrupted!");
				if (m_pLast != nullptr)
					Raise("The FlexibleList is corrupted!");

				m_pFirst = new Node;
				m_pLast= new Node;

				m_pFirst->pData = std::make_shared<_Ty>(element);

				m_pFirst->pNext = m_pLast;
				m_pLast->pPrev = m_pFirst;
			}
			else {
				m_pLast->pData = std::make_shared<_Ty>(element);
				m_pLast->pNext = new Node;
				m_pLast->pNext->pPrev = m_pLast;
				m_pLast = m_pLast->pNext;
			}
			++m_Size;
		}
		void AddRange(const FlexibleList<_Ty>& elements) {
			for (uInt i = 0; i < elements.Size(); ++i)
				Add(elements[i]);
		}
		void Insert(const uInt& index, const _Ty& element);

		void Remove(const uInt& index) {
			if (index >= m_Size)
				Raise("FlexibleList: Out of range");

			if (m_Size == 1) {
				SAFE_DELETE(&m_pFirst);
				SAFE_DELETE(&m_pLast);
				return;
			}

			Node* pIterator = nullptr;

			const Bool indexNearEnd = ((m_Size - index - 1) < index);
			if (indexNearEnd) {
				pIterator = m_pLast;
				for (uInt i = 0; i < (m_Size - index - 1); ++i)
					pIterator = pIterator->pPrev;
			}
			else {
				pIterator = m_pFirst;
				for (uInt i = 0; i < index; ++i)
					pIterator = pIterator->pNext;
			}

			if (pIterator == m_pLast) {
				m_pLast = pIterator->pPrev;
				SAFE_DELETE(&pIterator);
			}
			else if (pIterator == m_pFirst) {
				m_pFirst = m_pFirst->pNext;
				SAFE_DELETE(&pIterator);
			}
			else {
				pIterator->pPrev->pNext = pIterator->Next;
				pIterator->pNext->pPrev = pIterator->pPrev;
				SAFE_DELETE(&pIterator);
			}
			--m_Size;
		}
		void RemoveRange(const uInt& start, uInt end) {
			if (end == uInt(-1))
				end = m_Size;

			if (start == end) {
				if (m_IsStrickEnabled)
					Raise("Parameter \"start\" equals \"end\"!");
				Log::Warning("Parameter \"start\" equals \"end\"!");
				return;
			}

			if (start > end)
				Raise("Parameter \"start\" greater \"end\"!");
			else if (start >= m_Size)
				Raise("Parameter \"start\" out of range.");
			else if (end > m_Size)
				Raise("Parameter \"end\" out of range.");

			Node* pBeginNode = m_pFirst;
			for (uInt i = 0; i < start; ++i)
				pBeginNode = pBeginNode->pNext;

			if (pBeginNode == m_pFirst)
				m_pFirst = nullptr;

			Node* pEndNode = pBeginNode;
			for (uInt i = start; i < end; ++i) {
				pEndNode = pEndNode->pNext;
				if (pEndNode == m_pLast) {
					m_pLast = nullptr;
					SAFE_DELETE(&pEndNode);
				}
				else {
					SAFE_DELETE(&pEndNode->pPrev);
				}
				--m_Size;
			}

			if (m_pLast != nullptr)
				m_pFirst = pEndNode;
		}
		void RemoveRange(const uInt& end) {
			RemoveRange(Start, end);
		}
		void RemoveSome(const FlexibleList<uInt>& indices);
		void RemoveDuplicate();
		void Clear() {
			if (m_Size == 0) {
				if (m_IsStrickEnabled)
					Raise("The list cannot be cleared because it is empty.");
				Log::Warning("The list cannot be cleared because it is empty.");
				return;
			}
			RemoveRange(Start, End);
		}

		void Swap(const uInt& index_1, const uInt& index_2);
		void Change(const uInt& index, const _Ty& newElement);
		void Inverse();

		void SaveReservedCopy();
		void LoadReservedCopy();

		void AddBehavior(const Behavior& behavior);
		void RemoveBehavior(const Behavior& behavior);
		void ClearBehavior();

		uInt Find(const _Ty& element);
		FlexibleList<uInt> FindSome(const FlexibleList<_Ty>& elements);
		FlexibleList<_Ty> FindRange(const FlexibleList<_Ty>& elements);

		FlexibleList<_Ty> Copy();
		FlexibleList<_Ty> CopyRange();

		FlexibleList<_Ty> GetRange(const uInt& end);
		FlexibleList<_Ty> GetRange(const uInt& start, const uInt& end) {
			if (end == uInt(-1))
				end = m_Size;

			if (start == end) {
				if (m_IsStrickEnabled)
					Raise("Parameter \"start\" equals \"end\"!");

				Log::Warning("Parameter \"start\" equals \"end\"!");
				return FlexibleList();
			}

			if (start >= m_Size)
				Raise("Parameter \"start\" out of range.");
			else if (end >= m_Size)
				Raise("Parameter \"end\" out of range.");
			else if (start > end)
				Raise("Parameter \"start\" greater \"end\"!");

			for (uInt i = start; i < end; ++i) {

			}
		}


#ifdef _LIST_
		std::list<_Ty> ToStdList();
#endif
#ifdef _VECTOR_
		std::vector<_Ty> ToVector();
#endif
#ifdef _STACK_
		std::stack<_Ty> ToStack();
#endif
#ifdef _SET_
		std::set<_Ty> ToSet();
#endif
#ifdef _UNORDERED_SET_
		std::unordered_set<_Ty> ToUnorderedSet();
#endif

		_Ty& operator [] (const uInt& index) const {
			if (index >= m_Size)
				Raise("FlexibleList: Out of range");

			Node* nodePtr = m_pFirst;
			for (uInt i = 0; i < index; ++i) {
				if (nodePtr == nullptr)
					Raise("The FlexibleList is corrupted!");
				nodePtr = nodePtr->pNext;
			}
			return *nodePtr->pData;
		}
		_Ty& operator [] (const uInt& index) {
			return const_cast<const FlexibleList<_Ty>&>(*this)[index];
		}
		FlexibleList<_Ty> operator = (const FlexibleList& elements) {
			if (m_Size > 0)
				Clear();
			Assign(elements);
			return *this;
		}

		uInt GetMinLimit() const noexcept {
			return m_MinLimit;
		}
		uInt GetMaxLimit() const noexcept {
			return m_MaxLimit;
		}
		Behavior GetBehavior() const noexcept {
			return m_Behavior;
		}

		void SetMinimumLimit(const uInt& minLimit);
		void SetMaximumLimit(const uInt& maxLimit);
		void SetBehavior(const Behavior& behavior);


		void EnableStrict() noexcept {
			m_IsStrickEnabled = true;
		}
		void DisableStrict() noexcept {
			m_IsStrickEnabled = false;
		}

		uInt Size() const noexcept {
			return m_Size;
		}
		Bool IsStrickEnabled() const noexcept {
			return m_IsStrickEnabled;
		}

	private:
		Node* m_pFirst = nullptr;
		Node* m_pLast = nullptr;
		uInt m_Size = 0;
		uInt m_MinLimit = Start;
		uInt m_MaxLimit = End;
		Bool m_IsStrickEnabled = false;
		Behavior m_Behavior = Behavior::NONE;
	};
}