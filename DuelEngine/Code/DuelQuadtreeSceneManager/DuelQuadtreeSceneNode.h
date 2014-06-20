//  [3/7/2013 OMEGA] created

#ifndef _DUELQUADTREESCENENODE_H_
#define _DUELQUADTREESCENENODE_H_

#include "DuelQuadtreeCommon.h"
#include "DuelQuadtree.h"
#include "DuelSceneNode.h"

namespace Duel
{

	class DUELQUADTREE_API QuadtreeSceneNode : public DSceneNode
	{
	DUEL_DECLARE_RTTI(QuadtreeSceneNode)
	public:
		QuadtreeSceneNode(QuadtreeSceneManager* creator, const DString& name);
		~QuadtreeSceneNode();
		// set the quadrant this node belongs to.
		// 设置所属的四叉树节点.
		INTERNAL void	setQuadrant(Quadrant* quadrant) { mQuadrant = quadrant; }
		// get the quadrant this node belongs to, if this node belongs to nothing,
		// a NULL will be returned.
		// 取得所属的四叉树节点.
		Quadrant*	getQuadrant() const { return mQuadrant; }
		// unbind quadrant this node attached to, the children will unbind, too.
		// 解除与四叉树的绑定, 子节点也同样会解绑.
		INTERNAL void	unbindQuadrant();
		// query whether the center of this node is in the specified box. since we are loose, only check the center.
		// 查询本节点是否在包围盒中.因为是松散树, 所以只检查中心.
		bool	isIn(const DAxisAlignedBox& box);

		// override : DNode------------------------------
		// 重载Node的函数
		DNode*	removeChild(uint32 index);
		// override : DNode------------------------------
		// 重载Node的函数
		DNode*	removeChild(const DString& name);
		// override : DNode------------------------------
		// 重载Node的函数
		DNode*	removeChild(DNode* n);
		// override : DNode------------------------------
		// 重载Node的函数
		void	removeAllChildren();
		// override : DSceneNode------------------------
		// it will no longer calculate the size of children.
		// 重载SceneNode的函数, 不再计算子节点的包围盒.
		void	updateBound();

	protected:
		// quadrant pointer
		Quadrant*	mQuadrant;
	};
}


#endif // !_DUELQUADTREESCENENODE_H_
