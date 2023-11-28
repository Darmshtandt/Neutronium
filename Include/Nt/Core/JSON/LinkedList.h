#pragma once

namespace Nt {
	class JSONLinkedList : public JSONContainer {
	private:
		struct Node {
			Node* Next = nullptr;
			Node* Prev = nullptr;
			JSONDataType* Data = nullptr;
		};

	public:
		JSONLinkedList() noexcept = default;
		NT_API ~JSONLinkedList() override;

		NT_API void Add(JSONDataType* Ptr);

		NT_API void Remove(JSONDataType* Ptr);
		NT_API void Remove(int Index);

		NT_API void Clear() override;

		NT_API JSONDataType*& operator [] (int Index) const;

		NT_API uInt GetDataType() const noexcept override;

		NT_API Node* GetNode(int Index) const;

		NT_API Node* First() const noexcept;
		NT_API Node* Last() const noexcept;
		NT_API uInt Size() const noexcept;

	private:
		Node* m_First = nullptr;
		Node* m_Last = nullptr;
		uInt m_Size = 0;

	private:
		NT_API void _RemoveNode(Node* pNode);
	};
}