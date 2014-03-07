//  [1/27/2013 OMEGA] created

#include "DuelOctreeCommon.h"
#include "DuelOctree.h"

namespace Duel
{



	Octant::Octant( Octant* parent ) :
		mbTerminal(true)
	{
		//initialize all children to null.
		for ( int i = 0; i < 2; i++ )
		{
			for ( int j = 0; j < 2; j++ )
			{
				for ( int k = 0; k < 2; k++ )
				{
					mChildren[ i ][ j ][ k ] = 0;
				}
			}
		}
	}

	Octant::~Octant()
	{
		// do nothing, be careful.
	}

	void Octant::addSceneNode( OctreeSceneNode* node )
	{
		mNodeList.push_back(node);
		node->setOctant(this);
		increaseNodeCount();
		
	}

	void Octant::removeSceneNode( OctreeSceneNode* node )
	{
		mNodeList.erase( std::find( mNodeList.begin(), mNodeList.end(), node ) );
		node->setOctant(NULL);
		decreaseNodeCount();
	}

	void Octant::setRegion( const DAxisAlignedBox& box )
	{
		mRegion = box;
	}

	const DAxisAlignedBox& Octant::getRegion() const
	{
		return mRegion;
	}

	bool Octant::isTwiceSize( const DAxisAlignedBox& box ) const
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

	void Octant::getChildIndex( const DAxisAlignedBox& box, uint32& x, uint32& y, uint32& z )
	{
		DVector3 cent = mRegion.getMaximum().midPoint(mRegion.getMinimum());
		DVector3 bCent = box.getMaximum().midPoint(box.getMinimum());
		x = (bCent.x > cent.x) ? 1 : 0;
		y = (bCent.y > cent.y) ? 1 : 0;
		z = (bCent.z > cent.z) ? 1 : 0;
	}

	bool Octant::isOctantExists( uint32 x, uint32 y, uint32 z )
	{
		assert(x < 2 && y < 2 && z < 2);
		return mChildren[x][y][z] != NULL;
	}

	Octant* Octant::createOctant( uint32 x, uint32 y, uint32 z )
	{
		assert(x < 2 && y < 2 && z < 2);
		if (mChildren[x][y][z] != NULL)
		{
			return mChildren[x][y][z];
		}
		Octant* nOct = new Octant(this);
		DAxisAlignedBox	region(BBE_Finite);
		DVector3 min = mRegion.getMinimum();
		DVector3 max = mRegion.getMaximum();
		DVector3 cent = mRegion.getCenter();
		DReal *pX, *pY, *pZ;
		pX = (x == 0) ? &(max.x) : &(min.x);
		pY = (y == 0) ? &(max.y) : &(min.y);
		pZ = (z == 0) ? &(max.z) : &(min.z);
		*pX = cent.x;
		*pY = cent.y;
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
		region.setExtent(min, max);
		nOct->setRegion(region);
		mChildren[x][y][z] = nOct;
		mbTerminal = false;
		return nOct;
	}

	void Octant::removeOctant( uint32 x, uint32 y, uint32 z )
	{
		assert(x < 2 && y < 2 && z < 2);
		if (mChildren[x][y][z] != NULL)
		{
			delete mChildren[x][y][z];
			mChildren[x][y][z] = NULL;
			// terminal test
			if (mChildren[0][0][0] == NULL &&
				mChildren[0][0][1] == NULL &&
				mChildren[0][1][0] == NULL &&
				mChildren[0][1][1] == NULL &&
				mChildren[1][0][0] == NULL &&
				mChildren[1][0][1] == NULL &&
				mChildren[1][1][0] == NULL &&
				mChildren[1][1][1] == NULL)
			{
				mbTerminal = true;
			}
		}
	}

}