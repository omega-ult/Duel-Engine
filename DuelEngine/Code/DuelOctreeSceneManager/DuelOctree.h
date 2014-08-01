//  [1/27/2013 OMEGA] created

#ifndef _DUELOCTREE_H_
#define _DUELOCTREE_H_

#include "DuelOctreeCommon.h"
#include "DuelOctreeSceneNode.h"

namespace Duel
{
	/** Octree datastructure for managing scene nodes.
	@remarks
	This is a loose octree implementation, meaning that each
	octant child of the octree actually overlaps it's siblings by a factor
	of .5.  This guarantees that any thing that is half the size of the parent will
	fit completely into a child, with no splitting necessary.
	*/
	// 松散八叉树, 普通八叉树因为不好处理交界处的物体定界问题(称为八叉树的"粘度"), 所以平常使用的是
	// 定界区间比较大(长宽高约为原划分空间1.5倍)的区域, 这样在兄弟节点之间会有重合的区域, 虽然在划分粒度上降低了,
	// 但是能够解决粘度问题.
	class DUELOCTREE_API Octant 
	{
	public:
		// if the Octree is the root, use NULL.
		// 如果这个是一个根节点, parent设置为NULL.
		Octant(Octant* parent);
		// this method will delete all children
		// 这个函数将删除所有子树.
		~Octant();

		// add a DSceneNode, no check for node's region. usually called by OctreeSceneManager.
		// 加入一个场景节点, 这个函数不会检查节点所处的区域, 通常由DSceneManager调用.
		void		addSceneNode(OctreeSceneNode* node);
		// remove a DSceneNode from this octree node.
		// 移除一个节点.
		void		removeSceneNode(OctreeSceneNode* node);
		// remove all nodes
		void		removeAllSceneNodes();
		// get the count of node attached to this octree node, including children's node count.
		// 得到节点总数, 这个值包括子区间的节点数和.
		size_t				getNodeCount() const { return mNodeCount; }
			
		// query whether the specified child exists.
		// 查询指定序号的子树是否存在.
		bool				isOctantExists(uint32 x, uint32 y, uint32 z);
		// create a octant with specified index, if it already existed, current one will be returned.
		// 创建一个子树,如果已经存在, 则返回当前已存在的值
		Octant*				createOctant(uint32 x, uint32 y, uint32 z);
		// get child octant.
		// 取得某个子树.
		Octant*				getOctant(uint32 x, uint32 y, uint32 z)
		{
			assert(x < 2 && y < 2 && z < 2);
			return mChildren[x][y][z];
		}
		// delete an octant
		// 删除某个子树
		void				removeOctant(uint32 x, uint32 y, uint32 z);

		// query whether this octant is leaf octant
		// 查询这个节点是否为叶节点
		bool				isTerminal() const { return mbTerminal; }

		// set the bounding region of this octant.
		// 设置这颗子树的定界区域. 
		INTERNAL void		setRegion(const DAxisAlignedBox& box);
		// get the bounding region of this octree node. 
		// 取得这颗子树的定界区域.
		const DAxisAlignedBox&	getRegion() const;

		// this method is used to degermine whether the specified box is fit into a child node of this octant.
		// usually called by DSceneManager.
		// this method is the core of loose octree, which gives a 'loose' result of containment testing,
		// 这个函数用来判断给定的AAB是否适合放进这个节点的子树里, 通常由DSceneManager调用.
		// 这个函数是八叉树'松散'的核心, 它能返回一个较为宽松的包容性测试结果
		bool				isTwiceSize(const DAxisAlignedBox& box) const;

		// this method is used for querying which child should be traversed to insert specified box,
		// usually called by DSceneManager.
		// 这个函数用于取得给定的AAB所在的子树序号, 通常由DSceneManager调用.
		void				getChildIndex(const DAxisAlignedBox& box, uint32& x, uint32& y, uint32& z);

		// used to traverse children
		// 用于遍历子树.
		typedef	std::list<OctreeSceneNode*>			OctreeSceneNodeList;
		typedef	ListIterator<OctreeSceneNodeList>	OctreeSceneNodeIterator;
		OctreeSceneNodeIterator	getSceneNodeIterator() { return OctreeSceneNodeIterator(mNodeList.begin(), mNodeList.end()); }
	protected:
		// Increments the overall node count of this octree and all its parents
		// 增加绑定到该子树的节点数目.
		inline void			increaseNodeCount()
		{
			mNodeCount++;
			if ( mParent != NULL ) mParent -> increaseNodeCount();
		};

		// Decrements the overall node count of this octree and all its parents
		// 减少绑定到该子树的节点数目.
		inline void			decreaseNodeCount()
		{
			mNodeCount--;
			if ( mParent != NULL ) mParent -> decreaseNodeCount();
		};
		// scene node storage.
		OctreeSceneNodeList	mNodeList;
		// parent pointer.
		Octant*				mParent;
		// seperate octree in x, y, z axis.
		Octant*				mChildren[2][2][2];
		// the bounding region of this octant.
		DAxisAlignedBox		mRegion;
		// record of node count
		size_t				mNodeCount;
		// indictate whether this octant is terminal.
		bool				mbTerminal;

	};
}

#endif