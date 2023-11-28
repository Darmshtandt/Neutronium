#include <vector>

#include <Nt/Core/Defines.h>
#include <Nt/Core/NtTypes.h>
#include <Nt/Core/String.h>
#include <Nt/Core/Utilities.h>
#include <Nt/Core/SelfDeletingPtr.h>

#include <Nt/Core/JSON/DataTypes.h>
#include <Nt/Core/JSON/Dict.h>
#include <Nt/Core/JSON/LinkedList.h>

namespace Nt {
// ============================================================================
//	JSONLinkedList
// ----------------------------------------------------------------------------
	JSONLinkedList::~JSONLinkedList() {
		Clear();
	}

	void JSONLinkedList::Add(JSONDataType* Ptr) {
		if (m_Size == 0) {
			m_First = new Node;
			m_Last = new Node;

			m_First->Data = Ptr;
			m_First->Next = m_Last;
			m_Last->Prev = m_First;
		}
		else {
			m_Last->Data = Ptr;
			m_Last->Next = new Node;
			m_Last->Next->Prev = m_Last;
			m_Last = m_Last->Next;
		}
		++m_Size;
	}

	void JSONLinkedList::Remove(JSONDataType* Ptr) {
		Node* Iterator = m_First;
		while (Iterator != m_Last) {
			if ((*Iterator->Data) == Ptr) {
				_RemoveNode(Iterator);
				return;
			}
		}
		Raise("Element not found");
	}
	void JSONLinkedList::Remove(int Index) {
		_RemoveNode(this->GetNode(Index));
	}

	void JSONLinkedList::Clear() {
		if (m_Size > 0) {
			while (m_First) {
				if (m_First->Data) {
					switch (m_First->Data->GetDataType()) {
					case JSONDataType::DICT:
					case JSONDataType::LIST:
						dynamic_cast<JSONContainer*>(m_First->Data)->Clear();
						break;
					}

					delete m_First->Data;
				}
				m_First = m_First->Next;
			}
			m_Last = nullptr;
			m_Size = 0;
		}
	}

	JSONDataType*& JSONLinkedList::operator[](int Index) const {
		return GetNode(Index)->Data;
	}

	uInt JSONLinkedList::GetDataType() const noexcept {
		return JSONDataType::LIST;
	}

	JSONLinkedList::Node* JSONLinkedList::GetNode(int Index) const {
		if (uInt(Index) >= m_Size)
			Raise("Out of range");

		Node* Iterator = m_First;
		for (uInt i = 0; i < uInt(Index); ++i) {
			Iterator = Iterator->Next;
			if (!Iterator)
				Raise("List is incorrect");
		}
		return Iterator;
	}

	JSONLinkedList::Node* JSONLinkedList::First() const noexcept {
		return m_First;
	}
	JSONLinkedList::Node* JSONLinkedList::Last() const noexcept {
		return m_Last;
	}
	uInt JSONLinkedList::Size() const noexcept {
		return m_Size;
	}

	void JSONLinkedList::_RemoveNode(Node* pNode) {
		pNode->Prev->Next = pNode->Next;
		pNode->Next->Prev = pNode->Prev;
		SAFE_DELETE(&pNode->Data);
		SAFE_DELETE(&pNode);
	}

	
// ============================================================================
//	JSONDict
// ----------------------------------------------------------------------------
	JSONDict::~JSONDict() {
		Clear();
	}

	void JSONDict::Remove(JSONDataType* Key) {
		for (auto Iterator = m_Nodes.begin(); Iterator != m_Nodes.end(); ++Iterator) {
			if ((*Iterator->Key) == Key) {
				_DeleteNode(&(*Iterator));
				m_Nodes.erase(Iterator);
			}
		}
		Raise("Element not found");
	}

	void JSONDict::Clear() {
		for (_Node& Pair : m_Nodes)
			_DeleteNode(&Pair);
		m_Nodes.clear();
	}

	JSONDataType* JSONDict::operator [] (const uInt& Key) const {
		return (*this)[TypeConvertToJSONData(Key)];
	}
	JSONDataType* JSONDict::operator [] (const Int& Key) const {
		return (*this)[TypeConvertToJSONData(Key)];
	}
	JSONDataType* JSONDict::operator [] (const Float& Key) const {
		return (*this)[TypeConvertToJSONData(Key)];
	}
	JSONDataType* JSONDict::operator [] (const std::string& Key) const {
		return (*this)[TypeConvertToJSONData(Key)];
	}
	JSONDataType* JSONDict::operator [] (JSONDataType* Key) const {
		for (_Node Pair : m_Nodes)
			if ((*Pair.Key) == Key)
				return Pair.Value;

		String ErrorMsg = "Element \"";
		switch (Key->GetDataType()) {
		case JSONDataType::STRING:
			ErrorMsg += dynamic_cast<JSONString*>(Key)->Data;
			break;
		case JSONDataType::INT:
			ErrorMsg += dynamic_cast<JSONInt*>(Key)->Data;
			break;
		case JSONDataType::FLOAT:
			ErrorMsg += dynamic_cast<JSONFloat*>(Key)->Data;
			break;
		case JSONDataType::BOOL:
			ErrorMsg += dynamic_cast<JSONBool*>(Key)->Data;
			break;
		}
		ErrorMsg += "\" not found";
		Raise(ErrorMsg.c_str());
	}

	JSONDataType*& JSONDict::operator [] (JSONDataType* Key) {
		for (_Node& Pair : m_Nodes)
			if ((*Pair.Key) == Key)
				return Pair.Value;

		_Node NewNode = { Key, nullptr };
		m_Nodes.push_back(NewNode);
		return m_Nodes[m_Nodes.size() - 1].Value;
	}

	uInt JSONDict::GetDataType() const noexcept {
		return JSONDataType::DICT;
	}

	JSONDataType*& JSONDict::GetFromValue(JSONDataType* Value) {
		for (_Node& Pair : m_Nodes)
			if ((*Pair.Value) == Value)
				return Pair.Key;
		Raise("Element not found");
	}

	std::vector<JSONDataType*> JSONDict::GetKeys() const {
		std::vector<JSONDataType*> Keys;
		for (_Node Pair : m_Nodes)
			Keys.push_back(Pair.Key);
		return Keys;
	}
	std::vector<JSONDataType*> JSONDict::GetValues() const {
		std::vector<JSONDataType*> Values;
		for (_Node Pair : m_Nodes)
			Values.push_back(Pair.Value);
		return Values;
	}

	Bool JSONDict::IsContained(const uInt& Data) const {
		return IsContained(TypeConvertToJSONData(Data));
	}
	Bool JSONDict::IsContained(const Int& Data) const {
		return IsContained(TypeConvertToJSONData(Data));
	}
	Bool JSONDict::IsContained(const Float& Data) const {
		return IsContained(TypeConvertToJSONData(Data));
	}
	Bool JSONDict::IsContained(const std::string& Data) const {
		return IsContained(TypeConvertToJSONData(Data));
	}
	Bool JSONDict::IsContained(JSONDataType* Data) const noexcept {
		for (_Node Pair : m_Nodes)
			if ((*Pair.Key) == Data)
				return true;
		return false;
	}

	void JSONDict::_DeleteNode(_Node* pNode) {
		if (pNode && pNode->Value) {
			switch (pNode->Value->GetDataType()) {
			case JSONDataType::DICT:
			case JSONDataType::LIST:
				dynamic_cast<JSONContainer*>(pNode->Value)->Clear();
				break;
			}
			delete pNode->Value;
		}
		delete pNode->Key;
	}
	
// ============================================================================
//	JSON
// ----------------------------------------------------------------------------
}