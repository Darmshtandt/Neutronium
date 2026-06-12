#pragma once

#include <Nt/Core/Log.h>
#include <Nt/Graphics/Ex/BaseComponent.h>
#include <unordered_map>
#include <cassert>

namespace NtEx {
	using namespace Nt;

	class Object;
	using ObjectPtr = std::unique_ptr<Object>;

	class Object : public Identifier {
	public:
		explicit Object(const ClassID& id, Object* pParent = nullptr) :
			Identifier(id)
		{
			if (pParent && pParent->_AddNode(this))
				m_pParent = pParent;
		}
		~Object() noexcept override = default;

		template <class _Ty>
		_Ty* AddComponent() {
			const ClassID id = Class<_Ty>::ID();
			if (!m_Components.contains(id))
				m_Components[id] = std::make_unique<_Ty>(this);
			return static_cast<_Ty*>(m_Components[id].get());
		}

		template <class _Ty>
		void RemoveComponent() {
			const ClassID id = Class<_Ty>::ID();
			Assert(m_Components[id], "Does not have a component");
			m_Components.erase(id);
		}

		template <class... _Components>
		NT_NODISCARD std::tuple<_Components*...> GetComponentsTuple() noexcept {
			return std::make_tuple(
				static_cast<_Components*>(
					GetComponent<_Components>())...);
		}

		template <class _Ty>
		NT_NODISCARD const _Ty* GetComponent() const noexcept {
			return _GetComponentImpl<_Ty>();
		}

		template <class _Ty>
		NT_NODISCARD _Ty* GetComponent() noexcept {
			return const_cast<_Ty*>(_GetComponentImpl<_Ty>());
		}

		template <class _Ty>
		NT_NODISCARD Bool HasComponent() const noexcept {
			return m_Components.contains(Class<_Ty>::ID());
		}

		void SetParent(Object* pParent) {
			if (m_pParent == pParent)
				return;

			if (m_pParent)
				m_pParent->_RemoveChild(this);

			if (pParent && pParent->_AddNode(this))
				m_pParent = pParent;
		}
		NT_NODISCARD const std::list<ObjectPtr>& Nodes() const noexcept {
			return m_Nodes;
		}

	protected:
		std::unordered_map<ClassID, std::unique_ptr<BaseComponent>> m_Components;
		std::list<ObjectPtr> m_Nodes;
		Object* m_pParent;

	private:
		template <class _Ty>
		NT_NODISCARD const _Ty* _GetComponentImpl() const noexcept {
			const auto it = m_Components.find(Class<_Ty>::ID());
			if (it == m_Components.cend())
				return nullptr;
			return static_cast<const _Ty*>(it->second.get());
		}

		NT_NODISCARD Bool _FindNode(Object* pObject) const noexcept {
			for (const std::unique_ptr<Object>& pNode : m_Nodes) {
				if (pNode.get() == pObject || pNode->_FindNode(pObject))
					return true;
			}
			return false;
		}
		NT_NODISCARD Bool _AddNode(NotNull<Object*> pObject) {
			if (_FindNode(pObject) || pObject->_FindNode(this)) {
				Log::Instance().Warning("Unable to set parent, new parent is a child of object");
				assert(0);
				return false;
			}

			m_Nodes.emplace_back(pObject);
			return true;
		}
		void _RemoveChild(NotNull<Object*> pObject) noexcept {
			for (auto it = m_Nodes.cbegin(); it != m_Nodes.cend(); ++it) {
				if (it->get() == pObject) {
					m_Nodes.erase(it);
					return;
				}
			}

			Log::Instance().Warning("Object is not a child element");
			assert(0);
		}
	};
};