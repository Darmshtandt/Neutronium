#pragma once

namespace Nt {
	class JSON {
	public:
		JSON() noexcept = default;
		~JSON() {
			Release();
		}

		void Parse(std::ifstream File) {
			std::string Code(
				std::istreambuf_iterator<char>(File.rdbuf()),
				std::istreambuf_iterator<char>());
			File.close();

			Release();

			std::stack<JSONContainer*> Contaiters;
			JSONContainer* pCurentContainer = nullptr;
			JSONDataType* PrevValue = nullptr;
			JSONDataType* CurrentValue = nullptr;
			bool IsKey = false;

			Int2D Position = { 0, 0 };
			for (auto Iterator = Code.begin(); Iterator != Code.end(); ++Iterator) {
				++Position.x;
				if ((*Iterator) == '\n') {
					Position.x = 0;
					++Position.y;
				}

				if (_IsSkipSymbol(*Iterator))
					continue;

				char Symbol = std::tolower(*Iterator);
				PrevValue = CurrentValue;
				CurrentValue = nullptr;

				if (Symbol == '{') {
					_IsDict(Iterator, Position, Contaiters, pCurentContainer, PrevValue, CurrentValue, IsKey);
				}
				else if (Symbol == '[') {
					_IsList(Iterator, Position, Contaiters, pCurentContainer, PrevValue, CurrentValue, IsKey);
				}
				else if (Symbol == '}' || Symbol == ']') {
					Contaiters.pop();
					if (Contaiters.size() > 0)
						pCurentContainer = Contaiters.top();

					if ((Iterator + 1) == Code.end())
						break;
					else if (*(Iterator + 1) == ',')
						++Iterator;
				}
				else {
					_IsValue(Iterator, Position, Contaiters, pCurentContainer, PrevValue, CurrentValue, IsKey);
				}
			}
		}

		void Release() {
			SAFE_DELETE(&m_pData);
		}

		JSONDataType* GetData() const {
			return m_pData;
		}

	private:
		JSONDataType* m_pData = nullptr;

	private:
		bool _IsSkipSymbol(const char& Symbol) const noexcept {
			const bool IsSpace = (Symbol == ' ');
			const bool IsNewLine = (Symbol == '\n');
			const bool IsReturn = (Symbol == '\r');
			const bool IsTab = (Symbol == '\t');
			return (IsSpace || IsNewLine || IsReturn || IsTab);
		}

		void _Raise(cString Message, const Int2D& Position) const {
			String Str = Message;
			Str += "\nLine: " + std::to_string(Position.y);
			Str += "\nColumn: " + std::to_string(Position.x);
			Raise(Str);
		}

		void _IsDict(std::string::iterator& Iterator, Int2D& Position, std::stack<JSONContainer*>& Contaiters, JSONContainer*& pCurentContainer, JSONDataType*& PrevValue, JSONDataType*& CurrentValue, bool& IsKey) {
			JSONDict* NewDict = new JSONDict;
			if (pCurentContainer) {
				switch (pCurentContainer->GetDataType()) {
				case JSONDataType::DICT:
					IsKey = false;
					if (!PrevValue)
						_Raise("JSON: Incorrect file", Position);

					dynamic_cast<JSONDict*>(pCurentContainer)->operator[](PrevValue) = NewDict;
					break;
				case JSONDataType::LIST:
					dynamic_cast<JSONLinkedList*>(pCurentContainer)
						->Add(NewDict);
					break;
				default:
					_Raise("JSON: Failed to determine container type", Position);
				}
			}
			else if (!m_pData) {
				m_pData = NewDict;
			}
			else {
				_Raise("JSON: pCurentContainer is nullptr", Position);
			}
			Contaiters.push(NewDict);
			pCurentContainer = NewDict;
		}
		void _IsList(std::string::iterator& Iterator, Int2D& Position, std::stack<JSONContainer*>& Contaiters, JSONContainer*& pCurentContainer, JSONDataType*& PrevValue, JSONDataType*& CurrentValue, bool& IsKey) {
			JSONLinkedList* NewList = new JSONLinkedList;
			if (pCurentContainer) {
				switch (pCurentContainer->GetDataType()) {
				case JSONDataType::DICT:
					IsKey = false;
					dynamic_cast<JSONDict*>(pCurentContainer)->operator[](PrevValue) = NewList;
					break;
				case JSONDataType::LIST:
					dynamic_cast<JSONLinkedList*>(pCurentContainer)->Add(NewList);
					break;
				default:
					_Raise("JSON: Failed to determine container type", Position);
				}
			}
			else if (!m_pData) {
				m_pData = NewList;
			}
			else {
				_Raise("JSON: pCurentContainer is nullptr", Position);
			}
			Contaiters.push(NewList);
			pCurentContainer = NewList;
		}
		void _IsValue(std::string::iterator& Iterator, Int2D& Position, std::stack<JSONContainer*>& Contaiters, JSONContainer*& pCurentContainer, JSONDataType*& PrevValue, JSONDataType*& CurrentValue, bool& IsKey) const {
			// Is string
			if ((*Iterator) == '\'' || (*Iterator) == '\"') {
				auto BeginString = ++Iterator;
				while ((*Iterator) != '\'' && (*Iterator) != '\"')
					++Iterator;

				CurrentValue = new JSONString(std::string(BeginString, Iterator).c_str());
				++Iterator;
			}
			// Is num
			else if ((*Iterator) == '-' || (*Iterator) == '+' || isdigit((*Iterator))) {
				auto BeginString = Iterator;
				if (!isdigit(*Iterator))
					++Iterator;

				bool fIsFloat = false;
				while (isdigit(*Iterator) || (*Iterator) == '.') {
					if ((*Iterator) == '.') {
						if (fIsFloat)
							_Raise("JSON: Incorrect file", Position);
						fIsFloat = true;
					}
					++Iterator;
				}

				if (BeginString == Iterator)
					_Raise("JSON: Incorrect file", Position);

				std::string Digit(BeginString, Iterator);
				if (fIsFloat)
					CurrentValue = new JSONFloat(std::stof(Digit));
				else
					CurrentValue = new JSONInt(std::stoi(Digit));
			}
			// Is bool
			else if ((*Iterator) == 't') {
				const char True[] = "true";
				for (uInt i = 1; i < strlen(True); ++i) {
					++Iterator;
					if ((*Iterator) != True[i])
						_Raise("JSON: Incorrect file", Position);
				}
				CurrentValue = new JSONBool(true);
			}
			// Is bool
			else if ((*Iterator) == 'f') {
				++Iterator;
				const char False[] = "false";
				for (uInt i = 1; i < strlen(False); ++i) {
					++Iterator;
					if ((*Iterator) != False[i])
						_Raise("JSON: Incorrect file", Position);
				}
				CurrentValue = new JSONBool(true);
			}
			// Is error
			else {
				_Raise("JSON: Incorrect file", Position);
			}

			if ((*Iterator) == '}' || (*Iterator) == ']')
				--Iterator;

			if (pCurentContainer->GetDataType() == JSONDataType::DICT) {
				IsKey = (!IsKey);
				if (IsKey) {
					while ((*Iterator) != ':') {
						if (!_IsSkipSymbol(*Iterator))
							_Raise("JSON: Incorrect file", Position);
						++Iterator;
					}
				}
				else {
					JSONDict* pDict = dynamic_cast<JSONDict*>(pCurentContainer);

					const bool IsDelete = pDict->IsContained(PrevValue);
					(*pDict)[PrevValue] = CurrentValue;
					if (IsDelete)
						SAFE_DELETE(&PrevValue);
				}
			}
			else if (pCurentContainer->GetDataType() == JSONDataType::LIST) {
				dynamic_cast<JSONLinkedList*>(pCurentContainer)->Add(CurrentValue);
				while ((*Iterator) != ',' && (*Iterator) != ']' && (*Iterator) != '\'' && (*Iterator) != '\"') {
					if (!_IsSkipSymbol(*Iterator))
						_Raise("JSON: Incorrect file", Position);
					++Iterator;
				}
				if ((*Iterator) == ']')
					--Iterator;
			}
			else {
				_Raise("JSON: pCurentContainer is nullptr", Position);
			}
		}
	};
}