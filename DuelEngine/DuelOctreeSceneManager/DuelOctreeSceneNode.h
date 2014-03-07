//  [1/18/2013 OMEGA] created

#ifndef _DUELOCTREESCENENODE_H_
#define _DUELOCTREESCENENODE_H_

#include "DuelOctreeCommon.h"
#include "DuelOctree.h"
#include "DuelSceneNode.h"

namespace Duel
{

	class DUELOCTREE_API OctreeSceneNode : public DSceneNode
	{
	DUEL_DECLARE_RTTI(OctreeSceneNode)
	public:
		OctreeSceneNode(DSceneManager* creator, const DString& name);
		~OctreeSceneNode();
		// set the octant this node belongs to.
		// 设置所属的八叉树节点.
		INTERNAL void	setOctant(Octant* octant) { mOctant = octant; }
		// get the octant this node belongs to, if this node belongs to nothing,
		// a NULL will be returned.
		// 取得所属的八叉树节点.
		Octant*	getOctant() const { return mOctant; }
		// unbind octant this node attached to, the children will unbind, too.
		// 解除与八叉树的绑定, 子节点也同样会解绑.
		INTERNAL void	unbindOctant();
		// query whether the center of this node is in the specified box.
		// 查询本节点是否在包围盒中.
		bool	isIn(const DAxisAlignedBox& box);

		// override : DNode------------------------------
		// 重载DNode的函数
		DNode*	removeChild(uint32 index);
		// override : DNode------------------------------
		// 重载DNode的函数
		DNode*	removeChild(const DString& name);
		// override : DNode------------------------------
		// 重载DNode的函数
		DNode*	removeChild(DNode* n);
		// override : DNode------------------------------
		// 重载DNode的函数
		void	removeAllChildren();
		// override : DSceneNode------------------------
		// it will no longer calculate the size of children.
		// 重载DSceneNode的函数, 不再计算子节点的包围盒.
		void	updateBound();

	protected:
		// octant pointer
		Octant*	mOctant;
	};
}

#endif