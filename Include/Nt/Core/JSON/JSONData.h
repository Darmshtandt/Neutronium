#pragma once

#include <memory>

namespace Nt {
	class JSONList;
	class JSONDict;

	enum JSONType {
		DICT, LIST, STRING, INT, FLOAT, BOOL
	};

	struct JSONData {
		template <typename _Ty>
		JSONData(const JSONType& type, const _Ty& data) :
			m_Type(type),
			m_Data(std::make_shared<void>(reinterpret_cast<void*>(new _Ty(data))))
		{
		}

		template <typename _Ty>
		operator _Ty() {
			return (_Ty)(m_Data.get());
		}

	private:
		std::shared_ptr<void> m_Data;
		JSONType m_Type;
	};
}