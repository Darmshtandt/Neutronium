// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <NtStdH.h>
#include <Nt/Graphics.h>
#include <Nt/Physics/Simplex.h>
#include <Nt/Physics/Collision.h>

namespace Nt {
#ifdef _DEBUG
	Bool MeshCollider::IsCollide(NotNull<ICollider*> pCollider) const {
		if (pCollider->Type == ICollider::TYPE_MESH)
			return GJK(*pCollider.DynamicCast<const MeshCollider*>());
		return false;
	}
#else
	Bool MeshCollider::IsCollide(ICollider* pCollider) const {
		if (pCollider->Type == ICollider::TYPE_MESH)
			return GJK(*dynamic_cast<const MeshCollider*>(pCollider));
		return false;
	}
#endif
}