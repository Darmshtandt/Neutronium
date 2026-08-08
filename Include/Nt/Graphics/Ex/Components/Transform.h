#pragma once

#include <Nt/Graphics/Ex/MatrixPlus.h>
#include <Nt/Graphics/Ex/Object.h>
#include <cassert>

namespace NtEx {
	using namespace Nt;

	template <typename _Ty, uInt Dimension>
	class TransformBase {
	public:
		using VectorT = Vector<_Ty, Dimension>;

	public:
		TransformBase() noexcept {
			m_Size.Fill(_Ty(1));
		}
		virtual ~TransformBase() noexcept = default;

		NT_NODISCARD _Ty Distance(const TransformBase& other) const noexcept {
			return (Position() - other.Position()).Length();
		}
		NT_NODISCARD _Ty Distance(const VectorT& position) const noexcept {
			return (Position() - position).Length();
		}

		NT_NODISCARD _Ty DistanceSquare(const TransformBase& other) const noexcept {
			return (Position() - other.Position()).LengthSquare();
		}
		NT_NODISCARD _Ty DistanceSquare(const VectorT& position) const noexcept {
			return (Position() - position).LengthSquare();
		}

		NT_NODISCARD VectorT CalculateAngle(const TransformBase& other) const noexcept {
			return (Position() - other.Position()).GetNormalize();
		}
		NT_NODISCARD VectorT CalculateAngle(const VectorT& position) const noexcept {
			return (m_LocalPosition - position).GetNormalize();
		}

		NT_NODISCARD VectorT Right() const noexcept {
			return Matrix<_Ty, Dimension>::Right(LocalToWorld());
		}
		NT_NODISCARD VectorT Up() const noexcept {
			return Matrix<_Ty, Dimension>::Up(LocalToWorld());
		}
		NT_NODISCARD VectorT Forward() const noexcept {
			return Matrix<_Ty, Dimension>::Forward(LocalToWorld());
		}

		TransformBase* LocalPosition(const VectorT& pos) noexcept {
			if (m_LocalPosition == pos)
				return this;

			m_LocalPosition = pos;
			_MarkDirty();
			return this;
		}
		TransformBase* Size(const VectorT& size) noexcept {
			if (m_Size == size)
				return this;

			m_Size = size;
			_MarkDirty();
			return this;
		}

		void Translate(const VectorT& offset) noexcept {
			if (offset.LengthSquare() == 0.f)
				return;

			m_LocalPosition += offset;
			_MarkDirty();
		}
		void Resize(const VectorT& size) noexcept {
			if (size.LengthSquare() == 0.f)
				return;

			m_Size += size;
			_MarkDirty();
		}
		TransformBase* SetParent(TransformBase* pParent) {
			if (m_pParent == pParent)
				return this;

			if (m_pParent)
				m_pParent->_RemoveChild(this);

			if (pParent && pParent->_AddChild(this))
				m_pParent = pParent;

			_MarkDirty();
			return this;
		}

		NT_NODISCARD const Matrix4x4& LocalToWorld() const noexcept {
			if (m_IsDirty) {
				_ComputeMatrix();
				m_IsDirty = false;
			}
			return m_LocalToWorld;
		}
		NT_NODISCARD const Matrix4x4& WorldToLocal() const noexcept {
			if (m_IsDirty) {
				_ComputeMatrix();
				m_IsDirty = false;
			}
			return m_WorldToLocal;
		}

		NT_NODISCARD const VectorT& LocalPosition() const noexcept {
			return m_LocalPosition;
		}
		NT_NODISCARD const VectorT& Size() const noexcept {
			return m_Size;
		}

		NT_NODISCARD VectorT Position() const noexcept {
			return Matrix<_Ty, Dimension>::Translation(LocalToWorld());
		}
		NT_NODISCARD VectorT Rotation() const noexcept {
			return Matrix<_Ty, Dimension>::ExtractRotation(LocalToWorld());
		}

		NT_NODISCARD Bool IsDirty() const noexcept {
			return m_IsDirty;
		}

	protected:
		mutable Matrix4x4 m_LocalMatrix;
		mutable Matrix4x4 m_LocalToWorld;
		mutable Matrix4x4 m_WorldToLocal;
		mutable Bool m_IsDirty = false;

		std::vector<TransformBase*> m_Childs;
		TransformBase* m_pParent = nullptr;
		VectorT m_LocalPosition;
		Quaternion m_LocalRotation = { 0.f, 0.f, 0.f, 1.f };
		VectorT m_Size;

	protected:
		void _ComputeMatrix() const noexcept {
			m_LocalMatrix = Matrix<_Ty, Dimension>::LocalToWorldLH(
				m_LocalPosition, m_LocalRotation, m_Size);

			if (m_pParent != nullptr)
				m_LocalToWorld = m_pParent->LocalToWorld() * m_LocalMatrix;
			else
				m_LocalToWorld = m_LocalMatrix;
			m_WorldToLocal = m_LocalToWorld.GetInverse();
		}
		void _MarkDirty() const noexcept {
			m_IsDirty = true;
			for (const auto& pChild : m_Childs)
				pChild->_MarkDirty();
		}

		NT_NODISCARD Bool _FindChild(TransformBase* pTransform) const noexcept {
			for (const TransformBase* pChild : m_Childs) {
				if (pTransform == pChild || pChild->_FindChild(pTransform))
					return true;
			}
			return false;
		}
		NT_NODISCARD Bool _AddChild(NotNull<TransformBase*> pTransform) {
			if (_FindChild(pTransform) || pTransform->_FindChild(this)) {
				Log::Instance().Warning("Unable to set parent, new parent is a child of Transform");
				assert(0);
				return false;
			}

			m_Childs.emplace_back(pTransform);
			return true;
		}
		void _RemoveChild(NotNull<TransformBase*> pTransform) noexcept {
			for (auto it = m_Childs.cbegin(); it != m_Childs.cend(); ++it) {
				if (*it == pTransform) {
					m_Childs.erase(it);
					return;
				}
			}

			Log::Instance().Warning("Transform is not a child element");
			assert(0);
		}
	};


	template <typename _Ty>
	class Transform2D final : public TransformBase<_Ty, 2>, public BaseComponent {
	public:
		using Vector = Vector2D<_Ty>;

	public:
		explicit Transform2D(Object* pOwner) :
			BaseComponent(pOwner, Class<Transform2D>::ID())
		{
		}
		~Transform2D() noexcept override = default;

		Transform2D* LocalRotation(const _Ty& angle) noexcept {
			if (m_LocalAngle == angle)
				return this;

			m_LocalAngle = angle;
			this->m_LocalRotation = Quaternion::GetFromEuler(Float3D(0.f, 0.f, m_LocalAngle));
			this->_MarkDirty();
			return this;
		}

		void Rotate(const _Ty& angle) noexcept {
			if (angle == static_cast<_Ty>(0))
				return;

			m_LocalAngle += angle;
			this->m_LocalRotation = Quaternion::GetFromEuler(Float3D(0.f, 0.f, m_LocalAngle));
			this->_MarkDirty();
		}

		NT_NODISCARD const _Ty& LocalRotation() const noexcept {
			return m_LocalAngle;
		}

	private:
		_Ty m_LocalAngle = static_cast<_Ty>(0);
	};

	template <typename _Ty>
	class Transform3D final : public TransformBase<_Ty, 3>, public BaseComponent {
	public:
		using Vector = Vector3D<_Ty>;

	public:
		explicit Transform3D(Object* pOwner) :
			BaseComponent(pOwner, Class<Transform3D>::ID())
		{
		}
		~Transform3D() noexcept override = default;

		Transform3D* LocalPosition(const Vector& pos) noexcept {
			return static_cast<Transform3D*>(
				TransformBase<_Ty, 3>::LocalPosition(pos));
		}
		Transform3D* Size(const Vector& size) noexcept {
			return static_cast<Transform3D*>(
				TransformBase<_Ty, 3>::Size(size));
		}
		Transform3D* SetParent(Transform3D* pParent) noexcept {
			return static_cast<Transform3D*>(
				TransformBase<_Ty, 3>::SetParent(pParent));
		}

		Transform3D* LocalRotation(const Quaternion& quaternion) noexcept {
			if (this->m_LocalRotation == quaternion)
				return this;

			this->m_LocalRotation = quaternion;
			this->_MarkDirty();
			return this;
		}
		Transform3D* LocalRotationEuler(const Vector& euler) noexcept {
			return LocalRotation(Quaternion::GetFromEuler(euler));
		}

		void Rotate(const Vector& deltaEuler) noexcept {
			if (deltaEuler.LengthSquare() == 0.f)
				return;

			const Quaternion quaternion = Quaternion::GetFromEuler(deltaEuler);
			this->m_LocalRotation = (quaternion * this->m_LocalRotation).GetNormalize();
			this->_MarkDirty();
		}

		NT_NODISCARD const Quaternion& LocalRotation() const noexcept {
			return this->m_LocalRotation;
		}
		NT_NODISCARD Vector LocalRotationEuler() const noexcept {
			return this->m_LocalRotation.ToEuler();
		}
		NT_NODISCARD Vector LocalPosition() const noexcept {
			return this->m_LocalPosition;
		}
		NT_NODISCARD Vector Size() const noexcept {
			return this->m_Size;
		}
	};


	template<typename _Ty, uInt Dimension>
	struct TransformSelector;

	template<typename _Ty>
	struct TransformSelector<_Ty, 2> {
		using Type = Transform2D<_Ty>;
	};

	template<typename _Ty>
	struct TransformSelector<_Ty, 3> {
		using Type = Transform3D<_Ty>;
	};

	template<typename _Ty, uInt Dimension>
	struct TransformSelector {
		using Type = TransformBase<_Ty, Dimension>;
	};

	template<typename _Ty, uInt Dimension>
	using Transform = typename TransformSelector<_Ty, Dimension>::Type;

	using TransformFloat2D = Transform2D<Float>;
	using TransformFloat3D = Transform3D<Float>;
}