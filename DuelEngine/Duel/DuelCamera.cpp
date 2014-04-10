//  [7/16/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelCamera.h"
#include "DuelPlane.h"
#include "DuelSceneManager.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DCamera, DObject);

	DCamera::DCamera( const DString& name ) :
		mName(name),
		mNear(0.1f),
		mFar(10000.0f),
		mFov(DMath::HALF_PI/2),
		mAspectRatio(1.333333333f),
		mTrackNode(NULL),
		mOrientation(DQuaternion::IDENTITY),
		mEyePosition(DVector3::ZERO),
		mProjType(PT_Perspective),
		mFocalLength(1.0f),
		mOrthoWidth(10.0f),
		mOrthoHeight(10.0f/1.333333333f)
	{
		setFixedYawAxis(true);
		mFrustumPlanes.reserve(6);
		mPoints.reserve(8);
		for (uint32 i = 0; i < 6; ++i)
		{
			mFrustumPlanes.push_back(DPlane());
			mPoints.push_back(DVector3());
		}
		// add 2 more points.
		mPoints.push_back(DVector3());
		mPoints.push_back(DVector3());
		markViewDirty();
		markProjDirty();
	}

	DCamera::~DCamera()
	{

	}


	const DPlane& DCamera::getClippingPlane( FrustumPlane fp )
	{
		uint32 index = (uint32)fp;
		assert(index < 6);
		if (index < 6)
		{
			return mFrustumPlanes.at(index);
		}
		else
		{
			return mFrustumPlanes.at(0);
		}
	}

	const DVector3& DCamera::getFrustumPoint( FrustumPoint fp )
	{
		uint32 index = (uint32)fp;
		assert(index < 8);
		if (index < 8)
		{
			return mPoints.at(index);
		}
		else
		{
			return mPoints.at(0);
		}
	}

	void DCamera::setNearClipDistance( DReal nearDist )
	{
		assert(nearDist < mFar);
		if (nearDist < mFar)
		{
			if(DMath::realEqual(nearDist, 0.0f))
			{
				nearDist = 0.01f;
			}
			else
			{
				mNear = nearDist;
			}
			markProjDirty();
		}
	}

	void DCamera::setFarClipDistance( DReal farDist )
	{
		assert(farDist > mNear);
		if (farDist > mNear)
		{
			mFar = farDist;
			markProjDirty();
		}
	}

	void DCamera::setOrthoWidth( DReal w )
	{
		assert(w > 0.0f);
		if (!DMath::realEqual(0.0f, w))
		{
			mOrthoWidth = w;
			markProjDirty();
		}
	}

	void DCamera::setOrthoHeight( DReal h )
	{
		assert(h > 0.0f);
		if (!DMath::realEqual(0.0f, h))
		{
			mOrthoHeight = h;
			markProjDirty();
		}
	}


	void DCamera::setFOV( const DRadian& fov )
	{
		if(fov == 0.0f)
		{
			mFov = DMath::HALF_PI/2;
		}
		else
		{
			mFov = fov;
		}
		markProjDirty();
	}

	void DCamera::setAspectRatio( DReal ratio )
	{
		mAspectRatio = ratio;
		markProjDirty();
	}

	DCamera::FrustumTestResult DCamera::isInside( DVector3 pos )
	{
		update();
		DPlane::Side s;
		for(uint32 i = 0; i < 6; ++i)
		{
			s = mFrustumPlanes.at(i).getSide(pos);
			if (s == DPlane::S_Negative)
			{
				return FTS_Out;
			}
			if (s == DPlane::S_OnPlane)
			{
				return FTS_Intersect;
			}
		}
		return FTS_In;
	}

	DCamera::FrustumTestResult DCamera::isInside( DAxisAlignedBox bound)
	{
		update();
		if (bound.isNull())
		{
			return FTS_Out;
		}
		if (bound.isInfinite())
		{
			return FTS_Intersect;
		}
		for(uint32 i = 0; i < 6; ++i)
		{
			DPlane::Side s = mFrustumPlanes.at(i).getSide(bound);
			if (s == DPlane::S_Negative)
			{
				return FTS_Out;
			}
			if (s == DPlane::S_Across)
			{
				return FTS_Intersect;
			}
		}
		return FTS_In;
	}

	DCamera::FrustumTestResult DCamera::isInside( DOrientedBox ob )
	{
		update();
		DAxisAlignedBox bound = ob.getBoundingBox();
		if (bound.isNull())
		{
			return FTS_Out;
		}
		if (bound.isInfinite())
		{
			return FTS_Intersect;
		}
		DMatrix4 obLocalTrans = ob.getLocalTransform();

		for(uint32 i = 0; i < 6; ++i)
		{
			// construct plane in ob's local space.
			DPlane P = mFrustumPlanes.at(i);
			DVector3 d = P.getPointOrthocenter(DVector3::ZERO);
			DVector3 n = P.normal;
			d = obLocalTrans * DVector4(d, 1.0f);
			n = obLocalTrans * DVector4(n, 0.0f);
			P = DPlane(n, d);

			DPlane::Side s = P.getSide(bound);
			if (s == DPlane::S_Negative)
			{
				return FTS_Out;
			}
			if (s == DPlane::S_Across)
			{
				return FTS_Intersect;
			}
		}
		return FTS_In;
	}

	DCamera::FrustumTestResult DCamera::isInside( DSphere sphere)
	{
		update();
		DPlane::Side s;
// 		DVector3 cent = sphere.getCenter();
// 		DPlane::Side centSide = DPlane::S_POSITIVE;
// 		bool centOut = false;
		for(uint32 i = 0; i < 6; ++i)
		{
			s = mFrustumPlanes.at(i).getSide(sphere);
			if (s == DPlane::S_Negative)
			{
				return FTS_Out;
			}
			if (s == DPlane::S_Across)
			{
				return FTS_Intersect;
			}
		}
		return FTS_In;

	}

	void DCamera::lookAt( const DVector3& dest )
	{
		setDirection(dest - mEyePosition);
	}

	void DCamera::lookAt( DReal x, DReal y, DReal z )
	{
		lookAt(DVector3(x, y, z));
	}

	void DCamera::setEyePosition( const DVector3& p )
	{
		mEyePosition = p;
		markViewDirty();
	}

	void DCamera::setEyePosition( DReal x, DReal y, DReal z )
	{
		setEyePosition(DVector3(x,y,z));
	}

	void DCamera::move( const DVector3& rV )
	{
		mEyePosition += rV;
		markViewDirty();
	}

	void DCamera::moveRelative( const DVector3& rV )
	{
		DVector3 trans = mOrientation*rV;
		mEyePosition += trans;
		markViewDirty();
	}

	void DCamera::roll( const DRadian& angle )
	{
		rotate(mOrientation*DVector3::UNIT_Z, angle);
	}

	void DCamera::yaw( const DRadian& angle )
	{
		DVector3 yAxis;
		if (mbFixedYaw)
		{
			yAxis = mFixedAxis;
		}
		else
		{
			yAxis = mOrientation * DVector3::UNIT_Y;
		}
		rotate(yAxis, angle);
	}

	void DCamera::pitch( const DRadian& angle )
	{
		rotate(mOrientation*DVector3::UNIT_X, angle);
	}

	void DCamera::rotate( const DVector3& axis, const DRadian& angle )
	{
		DQuaternion q;
		q.fromAngleAxis(angle, axis);
		rotate(q);
	}

	void DCamera::rotate( const DQuaternion& q )
	{
		// Note the order of the mult, i.e. q comes after

		// Normalise the quat to avoid cumulative problems with precision
		DQuaternion qnorm = q;
		qnorm.normalize();
		mOrientation =   mOrientation * qnorm;
		markViewDirty();
	}

	void DCamera::setDirection( const DVector3& d )
	{
		// Do nothing if given a zero vector
		// (Replaced assert since this could happen with auto tracking camera and
		//  camera passes through the lookAt point)
		if (d == DVector3::ZERO) return;
		
		DVector3	zAdj = d;
		zAdj.normalize();

		DQuaternion targetOrient;

		if (mbFixedYaw)
		{
			DVector3 xVec = mFixedAxis.crossProduct(zAdj);
			xVec.normalize();
			DVector3 yVec = zAdj.crossProduct(xVec);
			yVec.normalize();

			targetOrient.fromAxes(xVec, yVec, zAdj);
		}
		else
		{
			// Get axes from current quaternion
			DVector3 xVec, yVec, zVec;
			mOrientation.toAxes(xVec, yVec, zVec);
			DQuaternion qRot;
			if ((zVec+zAdj).squaredLength() < 0.00005f)
			{
				// Oops, a 180 degree turn (infinite possible rotation axes)
				// Default to yaw i.e. use current UP
				qRot.fromAngleAxis(DRadian(DMath::PI),yVec);
			}
			else
			{
				// Derive shortest arc to new direction
				qRot = zVec.getRotationTo(zAdj);
			}
			targetOrient = qRot * mOrientation;

		}
		mOrientation = targetOrient;
		markViewDirty();
	}

	void DCamera::setDirection( DReal x, DReal y, DReal z )
	{
		setDirection(DVector3(x,y,z));
	}

	void DCamera::setOrientation( const DQuaternion& orient )
	{
		mOrientation = orient;
		mOrientation.normalize();
		markViewDirty();
	}

	const DQuaternion& DCamera::getOrientation()
	{
		return	mOrientation;
	}

	DVector3 DCamera::getDirection() const
	{
		return	mOrientation * DVector3::UNIT_Z;
	}

	Duel::DVector3 DCamera::getUp() const
	{
		return	mOrientation * DVector3::UNIT_Y;
	}

	Duel::DVector3 DCamera::getRight() const
	{
		return	mOrientation * DVector3::UNIT_X;
	}

	void DCamera::setFixedYawAxis( bool useFixed, const DVector3& fixedAxis )
	{
		mbFixedYaw = useFixed;
		mFixedAxis = fixedAxis;
		markViewDirty();
	}

	const DMatrix4& DCamera::getViewMatrix()
	{
		update();
		return mViewMatrix;
	}

	const DMatrix4& DCamera::getInverseViewMatrix()
	{
		update();
		return mInvViewMatrix;
	}

	const DMatrix4& DCamera::getProjectionMatrix()
	{
		update();
		return mProjMatrix;
	}

	const DMatrix4& DCamera::getInverseProjectionMatrix()
	{
		update();
		return mInvProjMatrix;
	}



	void DCamera::update()
	{
		bool reCalPlanes = false;

		DVector3	translate;
		if (mTrackNode != NULL)
		{
			DVector3 cachePos = mTrackNode->getInheritedPosition();
			translate = cachePos + mEyePosition;
		}
		else
		{
			translate = mEyePosition;
		}

		if (mbViewDirty)
		{
			// update view matrix.
			mViewMatrix = DMath::makeViewMatrix(translate, mOrientation);
			mInvViewMatrix = mViewMatrix.getInverse();
			mbViewDirty = false;
			reCalPlanes = true;
		}
		if (mbProjDirty)
		{
			// update projecton matrix according to projection type
			if (mProjType == PT_OrthoGraphic)
			{
				// in mathematic, ortho projection matrix is :
				//	| 2/w,		0,		0,		0 |
				//	| 	0,	   2/h,		0,		0 |
				//	| 	0,		0,	1/(f-n), -n/(f-n) |
				//	|	0,		0,		0,		1 |
				// but we are using column-major storage, so traspose it.
				mProjMatrix[0][0] = 2.0f/mOrthoWidth; mProjMatrix[1][0] = 0.0f; mProjMatrix[2][0] = 0.0f; mProjMatrix[3][0] = 0.0f;
				mProjMatrix[0][1] = 0.0f; mProjMatrix[1][1] = 2.0f/mOrthoHeight; mProjMatrix[2][1] = 0.0f; mProjMatrix[3][1] = 0.0f;
				mProjMatrix[0][2] = 0.0f; mProjMatrix[1][2] = 0.0f; mProjMatrix[2][2] = 1.0f/(mFar- mNear); mProjMatrix[3][2] = -mNear/(mFar-mNear);
				mProjMatrix[0][3] = 0.0f; mProjMatrix[1][3] = 0.0f; mProjMatrix[2][3] = 0.0f; mProjMatrix[3][3] = 1.0f;

			}
			else // PT_PERSPECTIVE. 
			{
				// in mathematic, perspective projection matrix is :
				//	| cot(fov/2)/r,		0,		0,		0 |
				//	| 	0,	   cot(fov/2),	0,		0 |
				//	| 	0,		0,		f/(f-n),  -fn/(f-n)|
				//	|	0,		0,		1,		0 |
				// but we are using column-major storage, so traspose it.
				DReal cotFov2 = DMath::Cos(mFov/2) / DMath::Sin(mFov/2);
				mProjMatrix[0][0] = cotFov2/mAspectRatio; mProjMatrix[1][0] = 0.0f; mProjMatrix[2][0] = 0.0f; mProjMatrix[3][0] = 0.0f;
				mProjMatrix[0][1] = 0.0f; mProjMatrix[1][1] = cotFov2; mProjMatrix[2][1] = 0.0f; mProjMatrix[3][1] = 0.0f;
				mProjMatrix[0][2] = 0.0f; mProjMatrix[1][2] = 0.0f; mProjMatrix[2][2] = mFar/(mFar- mNear); mProjMatrix[3][2] = -(mFar*mNear)/(mFar-mNear);
				mProjMatrix[0][3] = 0.0f; mProjMatrix[1][3] = 0.0f; mProjMatrix[2][3] = 1.0f; mProjMatrix[3][3] = 0.0f;
			}
			mInvProjMatrix = mProjMatrix.getInverse();
			mbProjDirty = false;
			reCalPlanes = true;
		}

		if (reCalPlanes)
		{
			// update projecton matrix according to projection type
			if (mProjType == PT_OrthoGraphic)
			{
				// update points and planes
				DReal halfW = mOrthoWidth/2;
				DReal halfH = mOrthoHeight/2;
				mPoints[FP_NearLeftBottom]	= mInvViewMatrix * DVector3(-halfW, -halfH, mNear);
				mPoints[FP_NearLeftTop]		= mInvViewMatrix * DVector3(-halfW, halfH, mNear);
				mPoints[FP_NearRightTop]		= mInvViewMatrix * DVector3(halfW, halfH, mNear);
				mPoints[FP_NearRightBottom]	= mInvViewMatrix * DVector3(halfW, -halfH, mNear);
				mPoints[FP_FarRightTop]		= mInvViewMatrix * DVector3(halfW, halfH, mFar);
				mPoints[FP_FarLeftTop]		= mInvViewMatrix * DVector3(-halfW, halfH, mFar);
				mPoints[FP_FarLeftBottom]		= mInvViewMatrix * DVector3(-halfW, -halfH, mFar);
				mPoints[FP_FarRightBottom]	= mInvViewMatrix * DVector3(halfW, -halfH, mFar);

			}
			else // PT_PERSPECTIVE
			{
				DReal tanFov2 = DMath::Sin(mFov/2) / DMath::Cos(mFov/2);
				// near points.
				DReal halfH = mNear*tanFov2;
				DReal halfW = halfH * mAspectRatio;
				mPoints[FP_NearLeftBottom]	= mInvViewMatrix * DVector3(-halfW, -halfH, mNear);
				mPoints[FP_NearLeftTop]		= mInvViewMatrix * DVector3(-halfW, halfH, mNear);
				mPoints[FP_NearRightTop]		= mInvViewMatrix * DVector3(halfW, halfH, mNear);
				mPoints[FP_NearRightBottom]	= mInvViewMatrix * DVector3(halfW, -halfH, mNear);
				// far points.
				halfH = mFar*tanFov2;
				halfW = halfH * mAspectRatio;
				mPoints[FP_FarRightTop]		= mInvViewMatrix * DVector3(halfW, halfH, mFar);
				mPoints[FP_FarLeftTop]		= mInvViewMatrix * DVector3(-halfW, halfH, mFar);
				mPoints[FP_FarLeftBottom]		= mInvViewMatrix * DVector3(-halfW, -halfH, mFar);
				mPoints[FP_FarRightBottom]	= mInvViewMatrix * DVector3(halfW, -halfH, mFar);
			}
			// update planes.
			mFrustumPlanes[FP_Near]		= DPlane(mPoints[FP_NearLeftBottom], mPoints[FP_NearRightBottom], mPoints[FP_NearRightTop]);
			mFrustumPlanes[FP_Far]		= DPlane(mPoints[FP_FarLeftBottom], mPoints[FP_FarLeftTop], mPoints[FP_FarRightTop]);
			mFrustumPlanes[FP_Left]		= DPlane(mPoints[FP_NearLeftBottom], mPoints[FP_NearLeftTop], mPoints[FP_FarLeftTop]);
			mFrustumPlanes[FP_Right]	= DPlane(mPoints[FP_FarRightBottom], mPoints[FP_FarRightTop], mPoints[FP_NearRightTop]);
			mFrustumPlanes[FP_Top]		= DPlane(mPoints[FP_NearLeftTop], mPoints[FP_NearRightTop], mPoints[FP_FarRightTop]);
			mFrustumPlanes[FP_Bottom]	= DPlane(mPoints[FP_NearLeftBottom], mPoints[FP_FarLeftBottom], mPoints[FP_FarRightBottom]);
		}

	}

	void DCamera::setPorjectionType( ProjectionType pjt )
	{
		mProjType = pjt;
		markProjDirty();
	}


	DRay DCamera::getWorldRayFromCamera( const DRay& rayInCamera )
	{
		DRay ret;
		DMatrix4 invViewMat = getInverseViewMatrix();
		ret.setOrigin(invViewMat*rayInCamera.getOrigin());
		DVector3 vecTarget = rayInCamera.getPoint(1.0f);
		vecTarget = invViewMat*vecTarget;
		ret.pointTo(vecTarget);
		return ret;
	}

	DRay DCamera::getCameraRayFromViewport( const DViewport& vp, int32 x, int32 y )
	{
		DRay ret;
		ret.setOrigin(DVector3(0.0f, 0.0f, 0.0f));
		// point in screen space coordinate. ([-1, 1] range)
		DVector3 projPnt;
		DReal vpW = (DReal)vp.getWidth();
		DReal vpH = (DReal)vp.getHeight();
		DReal vpX = (DReal)x;
		DReal vpY = (DReal)y;
		projPnt.x = (vpX - vpW/2) / vpW * 2;
		projPnt.y = (vpY - vpH/2) / vpH * 2;
		projPnt.z = 0.5f;
		// get the inverse projection matrix to calculate point in the camera view space.
		DMatrix4 invProjMat = getProjectionMatrix();
		invProjMat = invProjMat.getInverse();
		DVector3 viewPnt = invProjMat * projPnt;

		ret. pointTo(viewPnt);
		return ret;
	}

	DRay DCamera::getCameraRayFromScreen( uint32 winWidth, uint32 windHeight, int32 x, int32 y )
	{
		return getCameraRayFromScreen(winWidth, windHeight, DVector2((DReal)x, (DReal)y));

	}

	Duel::DRay DCamera::getCameraRayFromScreen( uint32 winWidth, uint32 windHeight, const DVector2& pos )
	{
		DRay ret;
		ret.setOrigin(DVector3(0.0f, 0.0f, 0.0f));
		DVector3 projPnt;
		DReal winW = (DReal)winWidth;
		DReal winH = (DReal)windHeight;
		DReal winX = pos.x;
		DReal winY = pos.y;
		projPnt.x = winX / (winW * 0.5f);
		projPnt.y = winY / (winH * 0.5f);
		projPnt.z = 0.5f;
		// get the inverse projection matrix to calculate point in the camera view space.
		DMatrix4 invProjMat = getProjectionMatrix();
		invProjMat = invProjMat.getInverse();
		DVector3 viewPnt = invProjMat * projPnt;

		ret. pointTo(viewPnt);
		return ret;
	}

}