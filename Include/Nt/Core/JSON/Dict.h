#pragma once

namespace Nt {
	class JSONDict : public JSONContainer {
	private:
		struct _Node {
			JSONDataType* Key;
			JSONDataType* Value;
		};

	public:
		JSONDict() = default;
		NT_API ~JSONDict() override;

		NT_API void Remove(JSONDataType* Key);

		NT_API void Clear() override;

		NT_API JSONDataType* operator [] (const uInt& Key) const;
		NT_API JSONDataType* operator [] (const Int& Key) const;
		NT_API JSONDataType* operator [] (const Float& Key) const;
		NT_API JSONDataType* operator [] (const std::string& Key) const;
		NT_API JSONDataType* operator [] (JSONDataType* Key) const;

		NT_API JSONDataType*& operator [] (JSONDataType* Key);

		NT_API uInt GetDataType() const noexcept override;

		NT_API JSONDataType*& GetFromValue(JSONDataType* Value);

		NT_API std::vector<JSONDataType*> GetKeys() const;
		NT_API std::vector<JSONDataType*> GetValues() const;

		NT_API bool IsContained(const uInt& Data) const;
		NT_API bool IsContained(const Int& Data) const;
		NT_API bool IsContained(const Float& Data) const;
		NT_API bool IsContained(const std::string& Data) const;
		NT_API bool IsContained(JSONDataType* Data) const noexcept;

	private:
		std::vector<_Node> m_Nodes;

	private:
		NT_API void _DeleteNode(_Node* pNode);
	};
}