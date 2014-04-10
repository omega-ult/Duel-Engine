/*
	[7/15/2012 OMEGA] created
	Definition of camera class 
*/

#ifndef _DUELCAMERA_H_
#define _DUELCAMERA_H_

#include "DuelCommon.h"

namespace Duel {


	enum ProjectionType
	{
		PT_OrthoGraphic,
		PT_Perspective
	};

	enum FrustumPlane
	{
		FP_Near			= 0,
		FP_Far			= 1,
		FP_Left			= 2,
		FP_Right		= 3,
		FP_Top			= 4,
		FP_Bottom		= 5
	};

	enum FrustumPoint
	{
		FP_NearLeftBottom		= 0,
		FP_NearLeftTop		= 1,
		FP_NearRightTop		= 2,
		FP_NearRightBottom	= 3,
		FP_FarRightTop		= 4,
		FP_FarLeftTop			= 5,
		FP_FarLeftBottom		= 6,
		FP_FarRightBottom		= 7
	};

	// TODO : add reflection support?
	// The (x,y,z) coordinate system is assumed to be LEFT-HANDED.  Coordinate

	//             (+)
	//              y|  (+)|far|
	//               |  / z
	//               | /
	//               |/
	//      (-)------#--------(+) 
	//              /|       x
	//             / |
	//            /  |
	//    |near|(-)  |
	//              (-)

	class DUEL_API DCamera : public DObject
	{
	DUEL_DECLARE_RTTI(DCamera)
	public:
		DCamera(const DString& name);

		virtual ~DCamera();

		const DString&	getName() const { return mName; }
	
		// set/get near clip distance for near plane.
		virtual	void	setNearClipDistance(DReal nearDist);
		virtual	DReal	getNearClipDistance() const { return mNear; }

		// set/get far clip distance for far plane.
		virtual	void	setFarClipDistance(DReal farDist);
		virtual	DReal	getFarClipDistance() const { return mFar; }

		// set/get aspect ratio for the frustum.
		virtual	void	setAspectRatio(DReal ratio);
		virtual	DReal	getAspectRatio() const { return mAspectRatio; }


		typedef	VectorIterator<Planes>			FrustumPlaneIterator;
		FrustumPlaneIterator	getFrustumPlaneIterator() { update(); return FrustumPlaneIterator(mFrustumPlanes.begin(), mFrustumPlanes.end()); }
		typedef	std::vector<DVector3>			FrustumPoints;
		typedef	VectorIterator<FrustumPoints>	FrustumPointIterator;
		FrustumPointIterator	getFrustumPointIterator() { update(); return FrustumPointIterator(mPoints.begin(), mPoints.end()); }
		// get clipping plane.
		virtual	const DPlane&	getClippingPlane(FrustumPlane fp);

		// get frustum piont.
		virtual	const DVector3&	getFrustumPoint(FrustumPoint fp);

		// set/get projection type.
		virtual	void			setPorjectionType(ProjectionType pjt);
		virtual	ProjectionType	getProjectionType() const { return mProjType; }

		// set width/height for orthographic projection.
		virtual	void			setOrthoWidth(DReal w);
		virtual	void			setOrthoHeight(DReal h);
		virtual	DReal			getOrthoWidth() const { return mOrthoWidth; }
		virtual	DReal			getOrthoHeight() const { return mOrthoHeight; }

		// set/get fov. no zero value allowed.
		virtual	void			setFOV(const DRadian& fov);
		virtual	const DRadian&	getFOV() const { return mFov; }

		// make camera look at specified point.
		void			lookAt(const DVector3& dest);
		void			lookAt(DReal x, DReal y, DReal z);

		// set mEyePosition
		void			setEyePosition(const DVector3& p);
		void			setEyePosition(DReal x, DReal y, DReal z);
		DVector3			getEyePosition() const { return mEyePosition; }
			
		// move camera along world axes
		void			move(const DVector3& rV);
		// move camera along its own axes
		void			moveRelative(const DVector3& rV);


		// yaw camera anticlockwise, i.e along local y axis
		void			yaw(const DRadian& angle);
		// pitch camera anticlockwise,  i.e along local x axis
		void			pitch(const DRadian& angle);
		// roll camera anticlockwise, i.e along local z axis
		void			roll(const DRadian& angle);

		// rotate camera along specifiey axis, anticlockwise
		void			rotate(const DVector3& axis, const DRadian& angle);
		void			rotate(const DQuaternion& q);

		// after using these function, the camera's 'up' will not change
		// i.e, keep 'roll' being the same
		void			setDirection(const DVector3& d);
		void			setDirection(DReal x, DReal y, DReal z);

		// set/get orientation
		void				setOrientation(const DQuaternion& orient);
		const DQuaternion&	getOrientation();

		// get current direction
		DVector3			getDirection() const;
		// get current 'up'
		DVector3			getUp() const;
		// get current 'right'
		DVector3			getRight() const;

		// define the type of intersection testing result
		enum FrustumTestResult
		{
			FTS_In,
			FTS_Out,
			FTS_Intersect
		};
		// test whether the point is inside the frustum, parameter intersect
		// indicate result will be true even the box is just intersect with one plane.
		virtual	FrustumTestResult	isInside(DVector3 pos);
		// test whether the bounding box is inside the frustum
		virtual	FrustumTestResult	isInside(DAxisAlignedBox bound);
		// test whether the oriented box is inside the frustum
		virtual	FrustumTestResult	isInside(DOrientedBox ob);
		// test whether the bounding sphere is inside the frustum
		virtual	FrustumTestResult	isInside(DSphere sphere);

		// set fixed axis.
		virtual	void	setFixedYawAxis(bool useFixed, const DVector3& fixedAxis = DVector3::UNIT_Y);
		
		virtual	const DMatrix4&	getViewMatrix();
		virtual	const DMatrix4&	getInverseViewMatrix();
		virtual	const DMatrix4&	getProjectionMatrix();
		virtual	const DMatrix4&	getInverseProjectionMatrix();

		// these two methods are used in object picking.
		// get a ray in the world coordinate space, with given ray in camera space.
		// it simply calculate the ray multiplied by the inverse view matrix.
		DRay		getWorldRayFromCamera(const DRay& rayInCamera);
		// get a ray in the camera view coordinate space, with given x, y (to top-left corner) in the vp.
		DRay		getCameraRayFromViewport(const DViewport& vp, int32 x, int32 y);
		// point in screen using window's center point as origin,
		DRay		getCameraRayFromScreen(uint32 winWidth, uint32 windHeight, int32 x, int32 y);
		// point in screen using window's center point as origin,
		DRay		getCameraRayFromScreen(uint32 winWidth, uint32 windHeight, const DVector2& pos);

		// track position with a targetNode, only position affected.
		virtual void	trackNode(DSceneNode* targetNode) { mTrackNode = targetNode; }
		virtual	bool	isTrackingNode() const { return mTrackNode != NULL; }

		// update matrix and planes, if the camera is attached to a Node,
		// the position will also be updated.
		INTERNAL void	update();

	protected:
		
		// internal quick method to mark dirty.
		inline void		markViewDirty() { mbViewDirty = true; }
		inline void		markProjDirty() { mbProjDirty = true; }

		// name of this camera
		DString			mName;

		// projection type.
		ProjectionType	mProjType;
		// field-of-view
		DRadian			mFov;

		// width/height for orthographic projection
		DReal			mOrthoWidth;
		DReal			mOrthoHeight;

		// far clip distance
		DReal			mFar;
		// near clip distance
		DReal			mNear;
		// viewport ratio
		DReal			mAspectRatio;
		
		// track target.
		DSceneNode*		mTrackNode;

		// indicate whether the yaw.
		bool			mbFixedYaw;	
		// fixed yaw axis.
		DVector3			mFixedAxis;

		// position of eye.
		DVector3			mEyePosition;
		
		// Focal length of frustum (for stereo rendering, defaults to 1.0)
		DReal			mFocalLength;

		// use quaternion style to enhance performance
		DQuaternion		mOrientation;

		// projection matrix
		DMatrix4			mProjMatrix;
		DMatrix4			mInvProjMatrix;
		// view matrix
		DMatrix4			mViewMatrix;
		// inverse view matrix.
		DMatrix4			mInvViewMatrix;

		// 6 clipping planes.
		Planes			mFrustumPlanes;

		// 8 frustum points
		FrustumPoints	mPoints;

		// indicate that some values are changed. viewMatrix and
		// clipping planes should be recalculated.
		bool			mbViewDirty;
		bool			mbProjDirty;

	};
	

	// 使用listener模式, 监听Node的变化
// 	class DUEL_API CameraToNodeListener : public Node::EventListener
// 	{
// 	// 然后你懂的.
// 	// onUpdate.
// 	};

}

#endif