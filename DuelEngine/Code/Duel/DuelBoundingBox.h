//  [11/8/2012 OMEGA] created

#ifndef _DUELAXISALIGNEDBOX_H_
#define _DUELAXISALIGNEDBOX_H_

#include "DuelMath.h"
#include "DuelVector.h"
#include "DuelMatrix.h"

namespace Duel
{

	//             (+)                           maxPoint
	//              y|  (+)|far|		   5-----4
	//               |  / z		          /|    /|
	//               | /	             / |   / |
	//               |/		            1-----2  |
	//      (-)------#--------(+) 		|  6--|--7
	//              /|       x	        | /   | /
	//             / |	                |/    |/
	//            /  |		            0-----3
	//    |near|(-)  |             minPoint
	//              (-)


	enum BoundingBoxCorner
	{
		BBC_NearLeftBottom		= 0,
		BBC_NearLeftTop		= 1,
		BBC_NearRightTop		= 2,
		BBC_NearRightBottom	= 3,
		BBC_FarRightTop		= 4,
		BBC_FarLeftTop			= 5,
		BBC_FarLeftBottom		= 6,
		BBC_FarRightBottom		= 7,
		BBC_ForceUint			= 0xffffffffUL	// never used.
	};

	enum BoundingBoxExtent
	{
		BBE_Null,
		BBE_Finite,
		BBE_Infinite
	};

	class DUEL_API DAxisAlignedBox
	{
	public:
		DAxisAlignedBox();
		DAxisAlignedBox(BoundingBoxExtent e);
		DAxisAlignedBox(const DAxisAlignedBox& aabb);
		DAxisAlignedBox(const DVector3& minimum, const DVector3& maximum);
		DAxisAlignedBox(DReal minX, DReal minY, DReal minZ, DReal maxX, DReal maxY, DReal maxZ);

		DAxisAlignedBox&	operator =	(const DAxisAlignedBox& rBox);
		bool			operator ==	(const DAxisAlignedBox& rBox);
		bool			operator !=	(const DAxisAlignedBox& rBox);

		// query whether the box is a NULL box.
		inline bool		isNull() const { return mExtent == BBE_Null; }
		// query whether the box is a finite box.
		inline bool		isFinite() const { return mExtent == BBE_Finite; }
		// query whether the box is an INFINITE box.
		inline bool		isInfinite() const { return mExtent == BBE_Infinite; }
		// set the extent using two point. min and max.
		void			setExtent(const DVector3& minnimum, const DVector3& maximum);
		// set the extent using two point. min and max.
		void			setExtent(DReal minX, DReal minY, DReal minZ, DReal maxX, DReal maxY, DReal maxZ);
		// set the box to INFINITE
		void			setInfinite() { mExtent = BBE_Infinite; }
		// set the box to a NULL box.
		void			setNull()
		{ 
			mExtent = BBE_Null; 
			mMaximum = DVector3(DMath::REAL_NEG_INFINITY,DMath::REAL_NEG_INFINITY,DMath::REAL_NEG_INFINITY);
			mMinimum = DVector3(DMath::REAL_POS_INFINITY,DMath::REAL_POS_INFINITY,DMath::REAL_POS_INFINITY);
		}
		// set the box to a FINITE box.
		void			setFinite() { mExtent = BBE_Finite; }

		// get the two definition point.
		const DVector3&	getMinimum() const { return mMinimum; }
		const DVector3&	getMaximum() const { return mMaximum; }
		// set the two definition point.
		inline void		setMinimum(const DVector3& p) { mMinimum = p; }
		inline void		setMinimum(DReal minX, DReal minY, DReal minZ) { mMinimum.x = minX; mMinimum.y = minY; mMinimum.z = minZ; }
		inline void		setMaximum(const DVector3& p) { mMaximum = p; }
		inline void		setMaximum(DReal maxX, DReal maxY, DReal maxZ) { mMaximum.x = maxX; mMaximum.y = maxY; mMaximum.z = maxZ; }

		inline DVector3	getClosestPointTo(const DVector3& p) const
		{
			// "Push" p into the box on each dimension
			DVector3 r;
			if (p.x < mMinimum.x) {
				r.x = mMinimum.x;
			} else if (p.x > mMaximum.x) {
				r.x = mMaximum.x;
			} else {
				r.x = p.x;
			}
			if (p.y < mMinimum.y) {
				r.y = mMinimum.y;
			} else if (p.y > mMaximum.y) {
				r.y = mMaximum.y;
			} else {
				r.y = p.y;
			}
			if (p.z < mMinimum.z) {
				r.z = mMinimum.z;
			} else if (p.z > mMaximum.z) {
				r.z = mMaximum.z;
			} else {
				r.z = p.z;
			}
			// Return it
			return r;
		}

		// inline function used to quick set.
		inline void		setMinX(DReal x) { mMinimum.x = x; }
		inline void		setMinY(DReal y) { mMinimum.y = y; }
		inline void		setMinZ(DReal z) { mMinimum.z = z; }
		inline void		setMaxX(DReal x) { mMaximum.x = x; }
		inline void		setMaxY(DReal y) { mMaximum.y = y; }
		inline void		setMaxZ(DReal z) { mMaximum.z = z; }

		// get corner point.
		DVector3			getCorner(BoundingBoxCorner corner) const;
		DVector3			getCenter() const;
		// merge a box. 
		void			merge(const DAxisAlignedBox& rBox);
		inline void		merge( const DVector3& point )
		{
			switch (mExtent)
			{
			case BBE_Null: // if null, use this point
				setExtent(point, point);
				return;

			case BBE_Finite:
				mMaximum.ceil(point);
				mMinimum.floor(point);
				return;

			case BBE_Infinite: // if infinite, makes no difference
				return;
			}

			assert( false && "Never reached" );
		}

		// translate the box, without using expensive matrix.
		void			translate(const DVector3& trans);
		// scale the box, without using expensive matrix.
		void			scale(const DVector3& s);
		// transform the box. 
		void			transform(const DMatrix4& mat);
		// transform the box according to the affine matrix
		void			transformAffine(const DMatrix4& mat);
		// query whether the box isIntersected with the given one.
		bool			isIntersected(const DAxisAlignedBox& rBox) const;
		// get the intersected part.
		DAxisAlignedBox	getIntersection(const DAxisAlignedBox& rBox);
		// query whether the box completely contains the other one
		bool			contains(const DAxisAlignedBox& rBox);

		// calculate the volume of the box.
		DReal			getVolume() const;

		// get the size of the box, width in x, height in y, depth in z.
		DVector3			getSize() const;
		// get the half-size of the box
		DVector3			getHalfSize() const;

		// common value.
		static const	DAxisAlignedBox AAB_NULL;
		static const	DAxisAlignedBox AAB_INFINITE;
	protected:
		DVector3		mMinimum;
		DVector3		mMaximum;
		BoundingBoxExtent		mExtent;

	};




	// OrientedBox is constructed by using an axis-aligned box and an orient.
	// notice that OrientedBox is always finite.
	class DUEL_API DOrientedBox 
	{
	public:
		DOrientedBox();
		DOrientedBox(const DVector3& min, const DVector3& max, 
			const DVector3& origin = DVector3::ZERO, const DQuaternion& orient = DQuaternion::ZERO);
		// if aab is infinite, then this orient box will use a unit box.
		DOrientedBox(const DAxisAlignedBox& aab, 
			const DVector3& origin = DVector3::ZERO, const DQuaternion& orient = DQuaternion::ZERO);

		// translate the box, without using expensive matrix.
		void		translate(const DVector3& trans);
		// rotate the box along specifiey axis, anticlockwise
		void		rotate(const DVector3& axis, const DRadian& angle);
		void		rotate(const DQuaternion& q);

		void		setOrigin(const DVector3& origin);
		void		setOrientation(const DQuaternion& orient);

		const DAxisAlignedBox&	getBoundingBox() const { return mBox; }

		const DVector3&		getOrigin() const;
		const DQuaternion&	getOrientation() const;

		// get the two definition point.
		const DVector3&	getMinimum() const;
		const DVector3&	getMaximum() const;
		// set the two definition point.
		void		setMinimum(const DVector3& p);
		void		setMinimum(DReal minX, DReal minY, DReal minZ);
		void		setMaximum(const DVector3& p);
		void		setMaximum(DReal maxX, DReal maxY, DReal maxZ);


		// set bounding box's property
		void		setMinX(DReal x);
		void		setMinY(DReal y);
		void		setMinZ(DReal z);
		void		setMaxX(DReal x);
		void		setMaxY(DReal y);
		void		setMaxZ(DReal z);
		

		DVector3	getCorner( BoundingBoxCorner corner );
		
		DVector3	getClosestPointTo(const DVector3& p);

		// the local transform can transform objects from world space to its local space.
		const DMatrix4&	getLocalTransform();

	protected:
		void		update();
		void		markDirty() { mbDirty = true; }

		DAxisAlignedBox	mBox;
		DVector3		mOrigin;
		DQuaternion		mOrientation;
		
		DMatrix4		mLocalTransform;

		bool			mbDirty;

	};


}

#endif