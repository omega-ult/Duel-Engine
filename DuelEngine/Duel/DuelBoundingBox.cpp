//  [11/9/2012 OMEGA] created


#include "DuelCommon.h"
#include "DuelBoundingBox.h"

namespace Duel
{

	const DAxisAlignedBox DAxisAlignedBox::AAB_NULL;
	const DAxisAlignedBox DAxisAlignedBox::AAB_INFINITE(AAB_INFINITE);

	DAxisAlignedBox::DAxisAlignedBox()
	{
		setMinimum(-0.5f, -0.5f, -0.5f);
		setMaximum(0.5f, 0.5f, 0.5f);
		mExtent = BBE_Null;
	}

	DAxisAlignedBox::DAxisAlignedBox( BoundingBoxExtent e ) :
		mExtent(e)
	{
		setMinimum(-0.5f, -0.5f, -0.5f);
		setMaximum(0.5f, 0.5f, 0.5f);
	}

	DAxisAlignedBox::DAxisAlignedBox( const DAxisAlignedBox& rBox )
	{
		if (rBox.isNull())
		{
			setNull();
		}
		else if(rBox.isInfinite())
		{
			setInfinite();
		}
		else
		{
			setExtent(rBox.mMinimum, rBox.mMaximum);
			mExtent = rBox.mExtent;
		}
	}

	DAxisAlignedBox::DAxisAlignedBox( const DVector3& minimum, const DVector3& maximum )
	{
		setExtent(minimum, maximum);
		mExtent = BBE_Finite;
	}

	DAxisAlignedBox::DAxisAlignedBox( DReal minX, DReal minY, DReal minZ, DReal maxX, DReal maxY, DReal maxZ )
	{
		setExtent(minX, minY, minZ, maxX, maxY, maxZ);
		mExtent = BBE_Finite;
	}

	DAxisAlignedBox& DAxisAlignedBox::operator=( const DAxisAlignedBox& rBox )
	{
		if (rBox.isNull())
		{
			setNull();
		}
		else if(rBox.isInfinite())
		{
			setInfinite();
		}
		else
		{
			setExtent(rBox.mMinimum, rBox.mMaximum);
			mExtent = rBox.mExtent;
		}
		return *this;
	}

	bool DAxisAlignedBox::operator==( const DAxisAlignedBox& rBox )
	{
		if (mExtent == rBox.mExtent)
		{
			if (mExtent == BBE_Finite)
			{
				return (rBox.mMinimum == mMinimum) && (rBox.mMaximum == mMaximum);
			}
			return true;
		}
		return false;
	}

	bool DAxisAlignedBox::operator!=( const DAxisAlignedBox& rBox )
	{
		return !operator==(rBox);
	}

	void DAxisAlignedBox::setExtent( const DVector3& minimum, const DVector3& maximum )
	{
		setExtent(minimum.x, minimum.y, minimum.z, 
			maximum.x, maximum.y, maximum.z);
	}

	void DAxisAlignedBox::setExtent( DReal minX, DReal minY, DReal minZ, DReal maxX, DReal maxY, DReal maxZ )
	{
		mMinimum.x = minX < maxX ? minX : maxX;
		mMinimum.y = minY < maxY ? minY : maxY;
		mMinimum.z = minZ < maxZ ? minZ : maxZ;
		mMaximum.x = maxX > minX ? maxX : minX;
		mMaximum.y = maxY > minY ? maxY : minY;
		mMaximum.z = maxZ > minZ ? maxZ : minZ;
		mExtent = BBE_Finite;
	}

	DVector3 DAxisAlignedBox::getCorner( BoundingBoxCorner corner ) const
	{
		switch(corner)
		{
		case BBC_NearLeftBottom:
			return mMinimum;
		case BBC_NearLeftTop:
			return DVector3(mMinimum.x, mMaximum.y, mMinimum.z);
		case BBC_NearRightTop:
			return DVector3(mMaximum.x, mMaximum.y, mMinimum.z);
		case BBC_NearRightBottom:
			return DVector3(mMaximum.x, mMinimum.y, mMinimum.z);
		case BBC_FarRightTop:
			return mMaximum;
		case BBC_FarLeftTop:
			return DVector3(mMinimum.x, mMaximum.y, mMaximum.z);
		case BBC_FarLeftBottom:
			return DVector3(mMinimum.x, mMinimum.y, mMaximum.z);
		case BBC_FarRightBottom:
			return DVector3(mMaximum.x, mMinimum.y, mMaximum.z);
		default:
			// should never reach.
			return DVector3::ZERO;
		}

	}

	DVector3 DAxisAlignedBox::getCenter() const
	{
		return DVector3( (mMinimum.x + mMaximum.x)*0.5f,
						(mMinimum.y + mMaximum.y)*0.5f,
						(mMinimum.z + mMaximum.z)*0.5f);
	}

	void DAxisAlignedBox::merge( const DAxisAlignedBox& rBox )
	{
		if ((rBox.mExtent == BBE_Null) || (mExtent == BBE_Infinite))
		{
			return;
		}
		if (rBox.mExtent == BBE_Infinite)
		{
			mExtent = BBE_Infinite;
		}
		else if (mExtent == BBE_Null)
		{
			setExtent(rBox.mMinimum, rBox.mMaximum);
			mExtent = rBox.mExtent;
		}
		else
		{
			mMinimum.floor(rBox.mMinimum);
			mMaximum.ceil(rBox.mMaximum);
		}
	}

	void DAxisAlignedBox::translate( const DVector3& trans )
	{
		mMaximum += trans;
		mMinimum += trans;
	}

	void DAxisAlignedBox::scale( const DVector3& s )
	{
		if(mExtent != BBE_Finite)
		{
			return;
		}
		// assumes centered on origin.
		setExtent(mMinimum * s, mMaximum * s);
	}

	void DAxisAlignedBox::transform( const DMatrix4& mat )
	{
		// Do nothing if current null or infinite
		if( mExtent != BBE_Finite )
			return;

		DVector3 oldMin, oldMax, currentCorner;

		// Getting the old values so that we can use the existing merge method.
		oldMin = mMinimum;
		oldMax = mMaximum;

		// reset
		setNull();

		// We sequentially compute the corners in the following order :
		// 0, 6, 5, 1, 2, 4 ,7 , 3
		// This sequence allows us to only change one member at a time to get at all corners.

		// For each one, we transform it using the matrix
		// Which gives the resulting point and merge the resulting point.

		// First corner 
		// min min min
		currentCorner = oldMin;
		merge( mat * currentCorner );

		// min,min,max
		currentCorner.z = oldMax.z;
		merge( mat * currentCorner );

		// min max max
		currentCorner.y = oldMax.y;
		merge( mat * currentCorner );

		// min max min
		currentCorner.z = oldMin.z;
		merge( mat * currentCorner );

		// max max min
		currentCorner.x = oldMax.x;
		merge( mat * currentCorner );

		// max max max
		currentCorner.z = oldMax.z;
		merge( mat * currentCorner );

		// max min max
		currentCorner.y = oldMin.y;
		merge( mat * currentCorner );

		// max min min
		currentCorner.z = oldMin.z;
		merge( mat * currentCorner ); 
	}

	void DAxisAlignedBox::transformAffine( const DMatrix4& mat )
	{
		assert(mat.isAffine());
		// do nothing if the current box is NULL or INFINITE
		if (mExtent != BBE_Finite)
		{
			return;
		}

		DVector3	center = getCenter();
		DVector3	halfSize = getHalfSize();

		DVector3 newCenter = mat.transformAffine(center);
		DVector3 newHalfSize(
			DMath::Abs(mat[0][0]) * halfSize.x + DMath::Abs(mat[0][1]) * halfSize.y + DMath::Abs(mat[0][2]) * halfSize.z, 
			DMath::Abs(mat[1][0]) * halfSize.x + DMath::Abs(mat[1][1]) * halfSize.y + DMath::Abs(mat[1][2]) * halfSize.z,
			DMath::Abs(mat[2][0]) * halfSize.x + DMath::Abs(mat[2][1]) * halfSize.y + DMath::Abs(mat[2][2]) * halfSize.z);

		setExtent(newCenter - newHalfSize, newCenter + newHalfSize);
	}

	bool DAxisAlignedBox::isIntersected( const DAxisAlignedBox& rBox ) const
	{
		// Early-fail for nulls
		if (this->isNull() || rBox.isNull())
			return false;

		// Early-success for infinites
		if (this->isInfinite() || rBox.isInfinite())
			return true;

		// Use up to 6 separating planes
		if (mMaximum.x < rBox.mMinimum.x)
			return false;
		if (mMaximum.y < rBox.mMinimum.y)
			return false;
		if (mMaximum.z < rBox.mMinimum.z)
			return false;

		if (mMinimum.x > rBox.mMaximum.x)
			return false;
		if (mMinimum.y > rBox.mMaximum.y)
			return false;
		if (mMinimum.z > rBox.mMaximum.z)
			return false;

		// otherwise, must be intersecting
		return true;
	}

	DAxisAlignedBox DAxisAlignedBox::getIntersection( const DAxisAlignedBox& rBox )
	{
		if (this->isNull() || rBox.isNull())
		{
			return DAxisAlignedBox();
		}
		else if (this->isInfinite())
		{
			return rBox;
		}
		else if (rBox.isInfinite())
		{
			return *this;
		}

		DVector3 intMin = mMinimum;
		DVector3 intMax = mMaximum;

		intMin.ceil(rBox.getMinimum());
		intMax.floor(rBox.getMaximum());

		// Check intersection isn't null
		if (intMin.x < intMax.x &&
			intMin.y < intMax.y &&
			intMin.z < intMax.z)
		{
			return DAxisAlignedBox(intMin, intMax);
		}

		return DAxisAlignedBox();
	}

	bool DAxisAlignedBox::contains( const DAxisAlignedBox& rBox )
	{
		if (rBox.isNull() || this->isInfinite())
			return true;

		if (this->isNull() || rBox.isInfinite())
			return false;

		return this->mMinimum.x <= rBox.mMinimum.x &&
			this->mMinimum.y <= rBox.mMinimum.y &&
			this->mMinimum.z <= rBox.mMinimum.z &&
			rBox.mMaximum.x <= this->mMaximum.x &&
			rBox.mMaximum.y <= this->mMaximum.y &&
			rBox.mMaximum.z <= this->mMaximum.z;
	}

	DReal DAxisAlignedBox::getVolume() const
	{
		switch(mExtent)
		{
		case BBE_Null:
			return 0.0f;
		case BBE_Finite:
			{
				DVector3 diff = mMaximum - mMinimum;
				return diff.x*diff.y*diff.z;
			}
		case BBE_Infinite:
			return DMath::POS_INFINITY;
		default:
			return 0.0f;
		}
	}

	DVector3 DAxisAlignedBox::getSize() const
	{
		switch(mExtent)
		{
		case BBE_Null:
			return DVector3::ZERO;
		case BBE_Finite:
			return mMaximum-mMinimum;
		case BBE_Infinite:
			return DVector3(DMath::POS_INFINITY,DMath::POS_INFINITY,DMath::POS_INFINITY);
		default:
			return DVector3::ZERO;
		}

	}

	DVector3 DAxisAlignedBox::getHalfSize() const
	{
		switch (mExtent)
		{
		case BBE_Null:
			return DVector3::ZERO;

		case BBE_Finite:
			return (mMaximum - mMinimum) * 0.5;

		case BBE_Infinite:
			return DVector3(
				DMath::POS_INFINITY,
				DMath::POS_INFINITY,
				DMath::POS_INFINITY);

		default: // shut up compiler
			assert( false && "Never reached" );
			return DVector3::ZERO;
		}
	}

	DOrientedBox::DOrientedBox()
	{
		mBox.setExtent(DVector3(-0.5f,-0.5f,-0.5f), DVector3(0.5f,0.5f,0.5f));
		mOrigin = DVector3::ZERO;
		mOrientation = DQuaternion::ZERO;
		markDirty();
	}


	// TODO:::
	DOrientedBox::DOrientedBox( const DVector3& min, const DVector3& max, 
		const DVector3& origin, const DQuaternion& orient ) :
		mOrigin(origin), 
		mOrientation(orient)
	{
		mBox.setExtent(min, max);
		mOrigin = DVector3::ZERO;
		mOrientation = orient;
		markDirty();
	}

	DOrientedBox::DOrientedBox( const DAxisAlignedBox& aab, 
		const DVector3& origin, const DQuaternion& orient ) :
		mOrigin(origin), 
		mOrientation(orient)
	{
		if (aab.isInfinite())
		{
			mBox.setExtent(DVector3(-0.5f,-0.5f,-0.5f), DVector3(0.5f,0.5f,0.5f));
		}
		markDirty();
	}

	void DOrientedBox::translate( const DVector3& trans )
	{
		mOrigin += trans;
		markDirty();
	}

	void DOrientedBox::rotate( const DVector3& axis, const DRadian& angle )
	{
		DQuaternion q;
		q.fromAngleAxis(angle, axis);
		rotate(q);
	}

	void DOrientedBox::rotate( const DQuaternion& q )
	{
		// Note the order of the mult, i.e. q comes after

		// Normalise the quat to avoid cumulative problems with precision
		DQuaternion qnorm = q;
		qnorm.normalize();
		mOrientation = qnorm * mOrientation;
		markDirty();
	}

	void DOrientedBox::setMaxZ( DReal z )
	{
		mBox.setMaxZ(z);
	}

	void DOrientedBox::setMaxY( DReal y )
	{
		mBox.setMaxY(y);
	}

	void DOrientedBox::setMaxX( DReal x )
	{
		mBox.setMaxX(x);
	}

	void DOrientedBox::setMinZ( DReal z )
	{
		mBox.setMinZ(z);
	}

	void DOrientedBox::setMinY( DReal y )
	{
		mBox.setMinY(y);
	}

	void DOrientedBox::setMinX( DReal x )
	{
		mBox.setMinX(x);
	}

	void DOrientedBox::setMaximum( DReal maxX, DReal maxY, DReal maxZ )
	{
		mBox.setMaximum(maxX, maxY, maxZ);
	}

	void DOrientedBox::setMaximum( const DVector3& p )
	{
		mBox.setMaximum(p);
	}

	void DOrientedBox::setMinimum( DReal minX, DReal minY, DReal minZ )
	{
		mBox.setMinimum(minX, minY, minZ);
	}

	void DOrientedBox::setMinimum( const DVector3& p )
	{
		mBox.setMinimum(p);
	}

	const DVector3& DOrientedBox::getMaximum() const
	{
		return mBox.getMaximum();
	}

	const DVector3& DOrientedBox::getMinimum() const
	{
		return mBox.getMinimum();
	}

	const DQuaternion& DOrientedBox::getOrientation() const
	{
		return mOrientation;
	}

	const DVector3& DOrientedBox::getOrigin() const
	{
		return mOrigin;
	}

	void DOrientedBox::setOrientation( const DQuaternion& orient )
	{
		mOrientation = orient;
		markDirty();
	}

	void DOrientedBox::setOrigin( const DVector3& origin )
	{
		mOrigin = origin;
	}

	void DOrientedBox::update()
	{
		if (mbDirty)
		{
			mLocalTransform = DMath::makeViewMatrix(mOrigin, mOrientation);
		}
	}

	const DMatrix4& DOrientedBox::getLocalTransform()
	{
		if (mbDirty)
		{
			update();
		}
		return mLocalTransform;
	}

	DVector3 DOrientedBox::getClosestPointTo( const DVector3& wP )
	{
		// "Push" p into the box on each dimension
		DMatrix4 wTolMat = getLocalTransform();
		DVector3 p = wTolMat * wP;
		DVector3 r;
		DReal minx = mBox.getMinimum().x;
		DReal miny = mBox.getMinimum().y; 
		DReal minz = mBox.getMinimum().z;
		DReal maxx = mBox.getMaximum().x;
		DReal maxy = mBox.getMaximum().y;
		DReal maxz = mBox.getMaximum().z;;
		if (p.x < minx) {
			r.x = minx;
		} else if (p.x > maxx) {
			r.x = maxx;
		} else {
			r.x = p.x;
		}
		if (p.y < miny) {
			r.y = miny;
		} else if (p.y > maxy) {
			r.y = maxy;
		} else {
			r.y = p.y;
		}
		if (p.z < minz) {
			r.z = minz;
		} else if (p.z > maxz) {
			r.z = maxz;
		} else {
			r.z = p.z;
		}
		// Return it
		r = wTolMat.getInverse() * r;
		return r;
	}

	DVector3 DOrientedBox::getCorner( BoundingBoxCorner corner )
	{
		DVector3 conerInLocal = mBox.getCorner(corner);
		return getLocalTransform().getInverse() * conerInLocal;
	}

}