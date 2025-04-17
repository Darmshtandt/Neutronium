#include <vector>

#include <Nt/Core/SelfDeletingPtr.h>

#include <Nt/Core/JSON/JSONDict.h>
#include <Nt/Core/JSON/JSONLinkedList.h>

namespace Nt {
// ============================================================================
//	JSONDict
// ----------------------------------------------------------------------------
	JSONDict::~JSONDict() {
		Clear();
	}

	void JSONDict::Remove(JSONData* pKey) {
		for (auto Iterator = m_Nodes.begin(); Iterator != m_Nodes.end(); ++Iterator) {
			if ((*Iterator->Key) == pKey) {
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

	JSONData* JSONDict::operator [] (const uInt& key) const {
		return (*this)[TypeConvertToJSONData(key)];
	}
	JSONData* JSONDict::operator [] (const Int& key) const {
		return (*this)[TypeConvertToJSONData(key)];
	}
	JSONData* JSONDict::operator [] (const Float& key) const {
		return (*this)[TypeConvertToJSONData(key)];
	}
	JSONData* JSONDict::operator [] (const std::string& key) const {
		return (*this)[TypeConvertToJSONData(key)];
	}
	JSONData* JSONDict::operator [] (JSONData* key) const {
		RequireNotNull(key);

		for (const _Node& pair : m_Nodes) {
			if ((*pair.Key) == key)
				return pair.Value;
		}

		String errorMsg = "Element \"";
		switch (key->GetType()) {
		case JSONData::STRING:
			errorMsg += dynamic_cast<JSONString*>(key)->Data;
			break;

		case JSONData::INT:
			errorMsg += dynamic_cast<JSONInt*>(key)->Data;
			break;

		case JSONData::FLOAT:
			errorMsg += dynamic_cast<JSONFloat*>(key)->Data;
			break;

		case JSONData::BOOL:
			errorMsg += dynamic_cast<JSONBool*>(key)->Data;
			break;
		}

		errorMsg += "\" not found";
		Raise(errorMsg.c_str());

		return nullptr;
	}

	JSONData*& JSONDict::operator [] (JSONData* key) {
		RequireNotNull(key);

		for (_Node& Pair : m_Nodes) {
			if ((*Pair.Key) == key)
				return Pair.Value;
		}

		_Node newNode = { key, nullptr };
		m_Nodes.push_back(newNode);

		return m_Nodes[m_Nodes.size() - 1].Value;
	}

	uInt JSONDict::GetType() const noexcept {
		return JSONData::DICT;
	}

	JSONData*& JSONDict::GetFromValue(JSONData* value) {
		RequireNotNull(value);

		for (_Node& pair : m_Nodes) {
			if ((*pair.Value) == value)
				return pair.Key;
		}

		Raise("Element not found");

		JSONData* nullPointer = nullptr;
		return nullPointer;
	}

	std::vector<JSONData*> JSONDict::GetKeys() const {
		std::vector<JSONData*> keys;
		for (_Node Pair : m_Nodes)
			keys.push_back(Pair.Key);

		return keys;
	}
	std::vector<JSONData*> JSONDict::GetValues() const {
		std::vector<JSONData*> values;
		for (_Node Pair : m_Nodes)
			values.push_back(Pair.Value);

		return values;
	}

	Bool JSONDict::IsContained(const uInt& data) const {
		return IsContained(TypeConvertToJSONData(data));
	}
	Bool JSONDict::IsContained(const Int& data) const {
		return IsContained(TypeConvertToJSONData(data));
	}
	Bool JSONDict::IsContained(const Float& data) const {
		return IsContained(TypeConvertToJSONData(data));
	}
	Bool JSONDict::IsContained(const std::string& data) const {
		return IsContained(TypeConvertToJSONData(data));
	}
	Bool JSONDict::IsContained(JSONData* data) const {
		RequireNotNull(data);

		for (_Node Pair : m_Nodes) {
			if ((*Pair.Key) == data)
				return true;
		}
		return false;
	}

	void JSONDict::_DeleteNode(_Node* pNode) {
		if (pNode == nullptr)
			return;

		if (pNode->Value != nullptr) {
			switch (pNode->Value->GetType()) {
			case JSONData::DICT:
			case JSONData::LIST:
				dynamic_cast<JSONContainer*>(pNode->Value)->Clear();
				break;
			}

			delete(pNode->Value);
		}

		delete(pNode->Key);
	}
}