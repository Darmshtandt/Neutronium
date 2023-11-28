#include <NtStdH.h>
#include <Nt/Graphics.h>
#include <Nt/Physics/RayCast.h>
#include <Nt/Physics/Simplex.h>
#include <Nt/Physics/Collision.h>

namespace Nt {
	Bool BoxCollider::IsCollide(const ICollider* pCollider) const {
		switch (pCollider->Type) {
		case TYPE_INVALID:
			Raise("Invalid collider type");
		case TYPE_BOX:
		{
			const BoxCollider Collider = *dynamic_cast<const BoxCollider*>(pCollider);
			return AABBCollisionCheck(
				Position,
				Position + m_Size,
				Collider.Position,
				Collider.Position + Collider.m_Size);
		}
		case TYPE_SPHERE:
		{
			const SphereCollider Sphere = *dynamic_cast<const SphereCollider*>(pCollider);
			return AABBCollisionCheck(
				Position,
				Position + m_Size,
				Sphere.Position - Sphere.Radius,
				Sphere.Position + Sphere.Radius);
		}
		//case TYPE_ELLIPSE:
		//	return false;
		//case TYPE_CAPSULE:
		//	return false;
		//case TYPE_TRIANGLE:
		//	return false;
		}
		Raise("Unknown collider type");
		return false;
	}

	Bool SphereCollider::IsCollide(const ICollider* pCollider) const {
		switch (pCollider->Type) {
		case TYPE_INVALID:
			Raise("Invalid collider type");
		case TYPE_BOX:
			return pCollider->IsCollide(this);
		case TYPE_SPHERE:
		{
			const SphereCollider Sphere = *dynamic_cast<const SphereCollider*>(pCollider);
			return ((Position - Sphere.Position).Length() <= (Radius + Sphere.Radius));
		}
		//case TYPE_ELLIPSE:
		//	return false;
		//case TYPE_CAPSULE:
		//	return false;
		//case TYPE_TRIANGLE:
		//	return false;
		}
		Raise("Unknown collider type");
		return false;
	}

	Bool TriangleCollider::IsCollide(const ICollider* pCollider) const {
		switch (pCollider->Type) {
		case TYPE_INVALID:
			Raise("Invalid collider type");
			//case TYPE_TRIANGLE:
			//{
			//	return false;
			//}
			//case TYPE_BOX:
			//	return false;
			//case TYPE_SPHERE:
			//	return false;
			//case TYPE_ELLIPSE:
			//	return false;
			//case TYPE_CAPSULE:
			//	return false;
		}
		Raise("Unknown collider type");
		return false;
	}

	Bool MeshCollider::IsCollide(const ICollider* pCollider) const {
		if (pCollider->Type == ICollider::TYPE_MESH)
			return GJK(*dynamic_cast<const MeshCollider*>(pCollider));
		return false;
	}
}