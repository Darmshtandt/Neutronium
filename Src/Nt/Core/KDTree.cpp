// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <Nt/Core/KDTree.h>

#include <algorithm>

namespace Nt {
	KDTree::Node::Node(HiperVector* pData, const Int& axis) :
		pData(RequireNotNull(pData)),
		Axis(axis),
		pLeft(nullptr),
		pRight(nullptr)
	{
	}
	KDTree::Node::~Node() {
		if (pData != nullptr)
			delete(pData);
	}

	NT_FORCE_INLINE Bool KDTree::Neighbor::operator < (const Neighbor& other) const noexcept {
		return SquareDistance < other.SquareDistance;
	}
	NT_FORCE_INLINE Bool KDTree::Neighbor::operator > (const Neighbor& other) const noexcept {
		return SquareDistance > other.SquareDistance;
	}


	KDTree::KDTree(const KDTree& tree) :
		m_pRoot(_CreateCopy(tree.m_pRoot))
	{
	}

	KDTree::KDTree(KDTree&& tree) noexcept : m_pRoot(tree.m_pRoot) {
		tree.m_pRoot = nullptr;
	}

	KDTree::~KDTree() {
		_DestroySubTree(m_pRoot);
	}


	KDTree& KDTree::operator = (const KDTree& tree) {
		if (this == &tree)
			return *this;

		_DestroySubTree(m_pRoot);
		m_pRoot = _CreateCopy(tree.m_pRoot);

		return *this;
	}

	KDTree& KDTree::operator=(KDTree&& tree) noexcept {
		if (this == &tree)
			return *this;

		_DestroySubTree(m_pRoot);
		m_pRoot = std::move(tree.m_pRoot);

		return *this;
	}

	void KDTree::_Build(const std::vector<HiperVector*>& points) {
		if (m_pRoot != nullptr)
			_DestroySubTree(m_pRoot);

		m_pRoot = _CreateSubTree(points, 0);
	}

	void KDTree::Destroy() {
		_DestroySubTree(m_pRoot);
	}

	void KDTree::RemoveSubTree(Node*& pTarget) {
		_DestroySubTree(pTarget);
	}

	HiperVector* KDTree::FindNearest(const HiperVector& target) const {
		Node* pBestNode = nullptr;
		Float minSquareDistance = FLT_MAX;

		_FindNearestSubTree(m_pRoot, target, pBestNode, minSquareDistance);

		return (pBestNode != nullptr) ? pBestNode->pData : nullptr;
	}
	KDTree::PriorityQueueData KDTree::FindKNearest(const HiperVector& target, const uInt& maxSize) const {
		PriorityQueueData queue;

		_FindKNearestSubTree(m_pRoot, target, maxSize, queue);

		return queue;
	}

	KDTree::Node* KDTree::_CreateCopy(Node* pRoot) {
		if (pRoot == nullptr)
			return nullptr;

		Node* pNode = new Node(pRoot->pData, pRoot->Axis);
		if (pRoot->pLeft != nullptr)
			pNode->pLeft = _CreateCopy(pRoot->pLeft);

		if (pRoot->pRight != nullptr)
			pNode->pRight = _CreateCopy(pRoot->pRight);

		return pNode;
	}

	KDTree::Node* KDTree::_CreateSubTree(std::vector<HiperVector*> points, const Int& depth) {
		if (points.empty() || points.front()->GetDimension() == 0)
			return nullptr;

		const Int axis = depth % points.front()->GetDimension();

		std::sort(points.begin(), points.end(), [axis](HiperVector* pLeft, HiperVector* pRight) {
			return pLeft->GetCoord(axis) < pRight->GetCoord(axis);
			});

		const Int median = points.size() / 2;

		std::vector<HiperVector*> leftPoints(points.begin(), points.begin() + median);
		std::vector<HiperVector*> rightPoints(points.begin() + median + 1, points.end());

		Node* pNode = new Node(points[median], axis);
		pNode->pLeft = _CreateSubTree(leftPoints, depth + 1);
		pNode->pRight = _CreateSubTree(rightPoints, depth + 1);

		return pNode;
	}

	void KDTree::_DestroySubTree(Node*& pNode) {
		if (pNode == nullptr)
			return;

		if (pNode->pLeft != nullptr)
			_DestroySubTree(pNode->pLeft);

		if (pNode->pRight != nullptr)
			_DestroySubTree(pNode->pRight);

		delete(pNode);
		pNode = nullptr;
	}

	void KDTree::_FindNearestSubTree(Node* pRoot, const HiperVector& target, Node*& pBestNode, Float& minSquareDistance) const {
		if (pRoot == nullptr || target.GetDimension() == 0)
			return;

		const Float squareDistanceKD = pRoot->pData->GetSquareDistance(target);
		if (minSquareDistance > squareDistanceKD) {
			pBestNode = pRoot;
			minSquareDistance = squareDistanceKD;

			if (minSquareDistance == 0)
				return;
		}

		const Int axis = pRoot->Axis;
		const Float targetToRoot = (target.GetCoord(axis) - pRoot->pData->GetCoord(axis));
		
		Node* pNext = (targetToRoot < 0) ? pRoot->pLeft : pRoot->pRight;
		Node* pOpposite = (targetToRoot < 0) ? pRoot->pRight : pRoot->pLeft;

		_FindNearestSubTree(pNext, target, pBestNode, minSquareDistance);

		if (targetToRoot * targetToRoot < minSquareDistance)
			_FindNearestSubTree(pOpposite, target, pBestNode, minSquareDistance);
	}
	void KDTree::_FindKNearestSubTree(Node* pRoot, const HiperVector& target, const uInt& maxSize, PriorityQueueData& queue) const {
		if (pRoot == nullptr || target.GetDimension() == 0)
			return;

		HiperVector*& pData = pRoot->pData;
		const Float squareDistanceKD = pData->GetSquareDistance(target);

		if (queue.size() < maxSize) {
			queue.push({ pData, squareDistanceKD });
		}
		else if (squareDistanceKD < queue.top().SquareDistance) {
			queue.pop();
			queue.push({ pData, squareDistanceKD });
		}

		const Int axis = pRoot->Axis;
		const Float targetToRoot = (target.GetCoord(axis) - pData->GetCoord(axis));
		const Float targetToRootSquare = targetToRoot * targetToRoot;

		Node* pNext = (targetToRoot < 0) ? pRoot->pLeft : pRoot->pRight;
		Node* pOpposite = (targetToRoot < 0) ? pRoot->pRight : pRoot->pLeft;

		_FindKNearestSubTree(pNext, target, maxSize, queue);

		if (queue.size() < maxSize || targetToRootSquare < queue.top().SquareDistance)
			_FindKNearestSubTree(pOpposite, target, maxSize, queue);
	}
	KDTree::Node* KDTree::_FindRoot(Node* pTarget, Node* pRoot) {
		if (pTarget == nullptr || pRoot == nullptr || pTarget == pRoot)
			return nullptr;

		if (pRoot->pLeft == pTarget || pRoot->pRight == pTarget)
			return pRoot;

		if (pRoot->Axis != pTarget->Axis) {
			Node* pNode = _FindRoot(pTarget, pRoot->pLeft);
			if (pNode == nullptr)
				return _FindRoot(pTarget, pRoot->pRight);
		}

		const Float& rootCoord = pRoot->pData->GetCoord(pRoot->Axis);
		const Float& targetCoord = pTarget->pData->GetCoord(pTarget->Axis);

		if (rootCoord < targetCoord)
			return _FindRoot(pTarget, pRoot->pRight);
		else if (targetCoord < rootCoord)
			return _FindRoot(pTarget, pRoot->pLeft);

		return nullptr;
	}
}