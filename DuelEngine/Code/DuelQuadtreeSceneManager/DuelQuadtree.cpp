//  [3/9/2013 OMEGA] created

#include "DuelQuadtreeCommon.h"
#include "DuelQuadtree.h"
#include "DuelQuadtreeSceneNode.h"


namespace Duel
{


	Quadrant::Quadrant( Quadrant* parent ) :
		mParent(parent),
		mbTerminal(true),
		mNodeCount(0)
	{
		//initialize all children to null.
		for ( int i = 0; i < 2; i++ )
		{
			for ( int j = 0; j < 2; j++ )
			{
				mChildren[ i ][ j ] = 0;
			}
		}
	}

	Quadrant::~Quadrant()
	{
		removeAllSceneNodes();
	}

	void Quadrant::addSceneNode( QuadtreeSceneNode* node )
	{
		mNodeList.push_back(node);
		node->setQuadrant(this);
		increaseNodeCount();

	}

	void Quadrant::removeSceneNode( QuadtreeSceneNode* node )
	{
		mNodeList.erase( std::find( mNodeList.begin(), mNodeList.end(), node ) );
		node->setQuadrant(NULL);
		decreaseNodeCount();
	}

	void Quadrant::setRegion( const DAxisAlignedBox& box )
	{
		mRegion = box;
		mRegion.setMinY(DMath::REAL_NEG_INFINITY);
		mRegion.setMaxY(DMath::REAL_POS_INFINITY);
// 		float offset = box.getHalfSize().x/2;
// 		DVector3 tMin = box.getMinimum();
// 		DVector3 tMax = box.getMaximum();
// 		tMin.x -= offset; tMin.z -= offset;
// 		tMax.x += offset; tMax.z += offset;
// 		mTestingRegion.setExtent(tMin, tMax);
// 		mTestingRegion.setMinY(DMath::NEG_INFINITY);
// 		mTestingRegion.setMaxY(DMath::POS_INFINITY);
	}

// 	const DAxisAlignedBox& Quadrant::getRegion() const
// 	{
// 		return mTestingRegion;
// 	}

	bool Quadrant::isTwiceSize( const DAxisAlignedBox& box ) const
	{
		if (box.isInfinite())
		{
			return false;
		}
		DVector3 halfBox = mRegion.getHalfSize();
		DVector3 boxSize = box.getSize();
		return ((boxSize.x <= halfBox.x) &&
			(boxSize.y <= halfBox.y) &&
			(boxSize.z <= halfBox.z));
	}

	void Quadrant::getChildIndex( const DAxisAlignedBox& box, uint32& x, uint32& z)
	{
		DVector3 cent = mRegion.getMaximum().midPoint(mRegion.getMinimum());
		DVector3 bCent = box.getMaximum().midPoint(box.getMinimum());
		x = (bCent.x > cent.x) ? 1 : 0;
		z = (bCent.z > cent.z) ? 1 : 0;
	}

	bool Quadrant::isQuadrantExists( uint32 x, uint32 z)
	{
		assert(x < 2 && z < 2);
		return mChildren[x][z] != NULL;
	}

	Quadrant* Quadrant::createQuadrant( uint32 x, uint32 z)
	{
		assert(x < 2 && z < 2);
		if (mChildren[x][z] != NULL)
		{
			return mChildren[x][z];
		}
		Quadrant* nQuad = new Quadrant(this);
		DAxisAlignedBox	region(BBE_Finite);
		DVector3 min = mRegion.getMinimum();
		DVector3 max = mRegion.getMaximum();
		DVector3 cent = mRegion.getCenter();
		DReal *pX, *pZ;
		pX = (x == 0) ? &(max.x) : &(min.x);
		pZ = (z == 0) ? &(max.z) : &(min.z);
		*pX = cent.x;
		*pZ = cent.z;
		// 		if (x == 0)
		// 		{
		// 			max.x = cent.x;
		// 		}
		// 		else
		// 		{
		// 			min.x = cent.x;
		// 		}
		// 		if (y == 0)
		// 		{
		// 			max.y = cent.y;
		// 		}
		// 		else
		// 		{
		// 			min.y = cent.y;
		// 		}
		// 		if (z == 0)
		// 		{
		// 			max.z = cent.z;
		// 		}
		// 		else
		// 		{
		// 			min.z = cent.z;
		// 		}
		min.y = DMath::REAL_NEG_INFINITY;
		max.y = DMath::REAL_POS_INFINITY;
		region.setExtent(min, max);
		nQuad->setRegion(region);
		mChildren[x][z] = nQuad;
		mbTerminal = false;
		return nQuad;
	}

	void Quadrant::removeQuadrant( uint32 x, uint32 z)
	{
		assert(x < 2 && z < 2);
		if (mChildren[x][z] != NULL)
		{
			delete mChildren[x][z];
			mChildren[x][z] = NULL;
			// terminal test
			if (mChildren[0][0] == NULL &&
				mChildren[0][1] == NULL &&
				mChildren[1][0] == NULL &&
				mChildren[1][1] == NULL)
			{
				mbTerminal = true;
			}
		}
	}

	void Quadrant::removeAllSceneNodes()
	{
		QuadtreeSceneNodeList::iterator i, iend = mNodeList.end();
		for (i = mNodeList.begin(); i != iend; i++)
		{
			(*i)->setQuadrant(NULL);
		}
		mNodeList.clear();
	}


}