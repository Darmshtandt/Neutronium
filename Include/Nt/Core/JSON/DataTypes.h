#pragma once

namespace Nt {
	struct JSONDataType {
		enum {
			NONE, DICT, LIST, STRING, INT, FLOAT, BOOL
		};

		Bool operator == (JSONDataType* DataType) noexcept;

		virtual uInt GetDataType() const noexcept {
			return NONE;
		}
	};

	struct JSONContainer : public JSONDataType {
		virtual ~JSONContainer() = 0;
		virtual void Clear() = 0;
	};

	__inline JSONContainer::~JSONContainer() {
	}

	struct JSONString : public JSONDataType {
		JSONString(const std::string& Value) noexcept :
			Data(Value.c_str()) {
		}
		JSONString(cString Value) noexcept :
			Data(Value) {
		}
		JSONString(const Char& Value) noexcept :
			Data(reinterpret_cast<cString>(Value)) {
		}

		uInt GetDataType() const noexcept override {
			return JSONDataType::STRING;
		}

		std::string Data;
	};

	struct JSONInt : public JSONDataType {
		JSONInt(const Int& Value) noexcept :
			Data(Value) {
		}

		uInt GetDataType() const noexcept override {
			return JSONDataType::INT;
		}

		Int Data;
	};

	struct JSONFloat : public JSONDataType {
		JSONFloat(const Float& Value) noexcept :
			Data(Value) {
		}

		uInt GetDataType() const noexcept override {
			return JSONDataType::FLOAT;
		}

		Float Data;
	};

	struct JSONBool : public JSONDataType {
		JSONBool(const Bool& Value) noexcept :
			Data(Value) {
		}

		uInt GetDataType() const noexcept override {
			return JSONDataType::BOOL;
		}

		Bool Data;
	};


	__inline Bool JSONDataType::operator == (JSONDataType* DataType) noexcept {
		if (this->GetDataType() == DataType->GetDataType()) {
			switch (this->GetDataType()) {
			case JSONDataType::INT:
				return (dynamic_cast<JSONInt*>(this)->Data ==
					dynamic_cast<JSONInt*>(DataType)->Data);
			case JSONDataType::FLOAT:
				return (dynamic_cast<JSONFloat*>(this)->Data ==
					dynamic_cast<JSONFloat*>(DataType)->Data);
			case JSONDataType::STRING:
				return (dynamic_cast<JSONString*>(this)->Data ==
					dynamic_cast<JSONString*>(DataType)->Data);
			case JSONDataType::BOOL:
				return (dynamic_cast<JSONBool*>(this)->Data ==
					dynamic_cast<JSONBool*>(DataType)->Data);
			}
		}
		return false;
	}


	__inline SelfDeletingPtr<JSONInt> TypeConvertToJSONData(const uInt& Value) {
		return new JSONInt(Value);
	}
	__inline SelfDeletingPtr<JSONInt> TypeConvertToJSONData(const int& Value) {
		return new JSONInt(Value);
	}
	__inline SelfDeletingPtr<JSONFloat> TypeConvertToJSONData(const float& Value) {
		return new JSONFloat(Value);
	}
	__inline SelfDeletingPtr<JSONString> TypeConvertToJSONData(const std::string& Value) {
		return new JSONString(Value);
	}
}