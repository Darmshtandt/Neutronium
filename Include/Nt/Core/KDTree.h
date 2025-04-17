#pragma once

#include <Nt/Core/Utilities.h>
#include <Nt/Core/Math/HiperVector.h>

namespace Nt {
	class KDTree {
	public:
		struct Node {
			NT_API Node(HiperVector* pData, const Int& axis);
			NT_API ~Node();

			HiperVector* pData;
			Int Axis;
			Node* pLeft = nullptr;
			Node* pRight = nullptr;
		};

	public:
		KDTree() noexcept = default;
		NT_API KDTree(const KDTree& tree);
		NT_API KDTree(KDTree&& tree) noexcept;

		template <class Data> requires std::is_base_of_v<HiperVector, Data>
		KDTree(const std::vector<Data>& points) {
			Build(points);
		}
		NT_API ~KDTree();

		template <class Data> requires std::is_base_of_v<HiperVector, Data>
		void Build(const std::vector<Data>& points) {
			std::vector<HiperVector*> data;
			for (const Data& point : points)
				data.push_back(new Data(point));

			_Build(data);
		}

		NT_API void Destroy();

		NT_API void RemoveSubTree(Node*& pTarget);

		template <class Data> requires std::is_base_of_v<HiperVector, Data>
		Data* FindNearest(const HiperVector& target) const {
			return reinterpret_cast<Data*>(FindNearest(target));
		}
		NT_API HiperVector* FindNearest(const HiperVector& target) const;

		NT_API KDTree& operator = (const KDTree& tree);
		NT_API KDTree& operator = (KDTree&& tree) noexcept;

	private:
		Node* m_pRoot = nullptr;

	private:
		NT_API void _Build(const std::vector<HiperVector*>& points);
		NT_API Node* _CreateCopy(Node* pRoot);

		NT_API _NODISCARD Node* _CreateSubTree(std::vector<HiperVector*> points, const Int& depth);
		NT_API void _DestroySubTree(Node*& pNode);
		NT_API void _FindNearestSubTree(Node* pNode, const HiperVector& target, Node*& pBestNode, Float& minSquareDistance) const;
		NT_API _NODISCARD Node* _FindRoot(Node* pTarget, Node* pRoot);
	};
}