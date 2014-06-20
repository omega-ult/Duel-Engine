//  [3/7/2013 OMEGA] created

#ifndef _DUELQUADTREE_H_
#define _DUELQUADTREE_H_

#include "DuelQuadtreeCommon.h"
#include "DuelQuadtreeSceneNode.h"

namespace Duel
{
	/** Quadtree datastructure for managing scene nodes.
	@remarks
	This is a loose Quadtree implementation, meaning that each
	quadrant child of the Quadtree actually overlaps it's siblings by a factor
	of .5.  This guarantees that any thing that is half the size of the parent will
	fit completely into a child, with no splitting necessary.
	*/
	// 松散四叉树, 普通四叉树因为不好处理交界处的物体定界问题(称为四叉树的"粘度"), 所以平常使用的是
	// 定界区间比较大(长宽高约为原划分空间1.5倍)的区域, 这样在兄弟节点之间会有重合的区域, 虽然在划分粒度上降低了,
	// 但是能够解决粘度问题.
	class DUELQUADTREE_API Quadrant 
	{
	public:
		// if the Quadtree is the root, use NULL.
		// 如果这个是一个根节点, parent设置为NULL.
		Quadrant(Quadrant* parent);
		// this method will delete all children
		// 这个函数将删除所有子树.
		~Quadrant();

		// add a SceneNode, no check for node's region. usually called by QuadtreeSceneManager.
		// 加入一个场景节点, 这个函数不会检查节点所处的区域, 通常由SceneManager调用.
		void		addSceneNode(QuadtreeSceneNode* node);
		// remove a SceneNode from this Quadtree node.
		// 移除一个节点.
		void		removeSceneNode(QuadtreeSceneNode* node);
		// get the count of node attached to this Quadtree node, including children's node count.
		// 得到节点总数, 这个值包括子区间的节点数和.
		size_t				getNodeCount() const { return mNodeCount; }
			
		// query whether the specified child exists.
		// 查询指定序号的子树是否存在.
		bool				isQuadrantExists(uint32 x, uint32 z);
		// create a Quadrant with specified index, if it already existed, current one will be returned.
		// 创建一个子树,如果已经存在, 则返回当前已存在的值
		Quadrant*				createQuadrant(uint32 x, uint32 z);
		// get child Quadrant.
		// 取得某个子树.
		Quadrant*				getQuadrant(uint32 x, uint32 z)
		{
			assert(x < 2 && z < 2);
			return mChildren[x][z];
		}
		// delete an quadrant
		// 删除某个子树
		void				removeQuadrant(uint32 x, uint32 z);

		// query whether this Quadrant is leaf Quadrant
		// 查询这个节点是否为叶节点
		bool				isTerminal() const { return mbTerminal; }

		// set the bounding region of this Quadrant.
		// 设置这颗子树的定界区域. 
		INTERNAL void		setRegion(const DAxisAlignedBox& box);
// 		// get the bounding region of this Quadtree node. 
// 		// 取得这颗子树的裁剪测试区域.
// 		const DAxisAlignedBox&	getRegion() const;
// 		
		// 取得这颗子树真正的定界区域.
		const DAxisAlignedBox&	getRegion() const { return mRegion; }

		// this method is used to degermine whether the specified box is fit into a child node of this Quadrant.
		// usually called by SceneManager.
		// this method is the core of loose Quadtree, which gives a 'loose' result of containment testing,
		// 这个函数用来判断给定的AAB是否适合放进这个节点的子树里, 通常由SceneManager调用.
		// 这个函数是四叉树'松散'的核心, 它能返回一个较为宽松的包容性测试结果
		bool				isTwiceSize(const DAxisAlignedBox& box) const;

		// this method is used for querying which child should be traversed to insert specified box,
		// usually called by SceneManager.
		// 这个函数用于取得给定的AAB所在的子树序号, 通常由SceneManager调用.
		void				getChildIndex(const DAxisAlignedBox& box, uint32& x, uint32& z);

		// used to traverse children
		// 用于遍历子树.
		typedef	std::list<QuadtreeSceneNode*>		QuadtreeSceneNodeList;
		typedef	ListIterator<QuadtreeSceneNodeList>	QuadtreeSceneNodeIterator;
		QuadtreeSceneNodeIterator	getSceneNodeIterator() { return QuadtreeSceneNodeIterator(mNodeList.begin(), mNodeList.end()); }
	protected:
		// Increments the overall node count of this Quadtree and all its parents
		// 增加绑定到该子树的节点数目.
		inline void			increaseNodeCount()
		{
			mNodeCount++;
			if ( mParent != NULL ) mParent -> increaseNodeCount();
		};

		// Decrements the overall node count of this Quadtree and all its parents
		// 减少绑定到该子树的节点数目.
		inline void			decreaseNodeCount()
		{
			mNodeCount--;
			if ( mParent != NULL ) mParent -> decreaseNodeCount();
		};
		// scene node storage.
		QuadtreeSceneNodeList	mNodeList;
		// parent pointer.
		Quadrant*				mParent;
		// seperate Quadtree in x, z, z axis.
		Quadrant*				mChildren[2][2];
		// the bounding region of this Quadrant.
		DAxisAlignedBox		mRegion;


		// record of node count
		size_t				mNodeCount;
		// indictate whether this Quadrant is terminal.
		bool				mbTerminal;

	};

}

#endif // !_DUELQUADTREE_H_
