//  [12/6/2012 OMEGA] created

#ifndef _DUELLIGHTSOURCE_H_
#define _DUELLIGHTSOURCE_H_

#include "DuelCommon.h"
#include "DuelColor.h"
#include "DuelRenderable.h"
#include "DuelMovable.h"

namespace Duel
{
	enum LightType
	{
		LT_Ambient,
		LT_Point,
		LT_Directional,
		LT_Spotlight,
		LT_SurfaceLight,
		LT_VolumeLight
	};

	class DUEL_API DLightSource : public DMovable
	{
	DUEL_DECLARE_RTTI(DLightSource)
	public:
		DLightSource(const DString& name);

		virtual LightType	getLightType() = 0;

		void			setShadowProperty(ShadowProperty sp);
		ShadowProperty	getShadowProperty() const;

		// set/get position of the light
		void			setPosition(DReal x, DReal y, DReal z);
		void			setPosition(const DVector3& pos);
		const DVector3&	getPosition() const;

		// set/get the color properties of the light.
		void			setDiffuseColor(DReal red, DReal green, DReal blue);
		void			setDiffuseColor(const DColor& color);
		const DColor&	getDiffuseColor() const;
		void			setSpecularColor(DReal red, DReal green, DReal blue);
		void			setSpecularColor(const DColor& color);
		const DColor&	getSpecularColor() const;

	protected:
		LightType		mType;
		ShadowProperty	mShadowProperty;
		DVector3		mPos;

		DColor			mDiffuseColor;
		DColor			mSpecularColor;

	};


	class DUEL_API DAmbientLight : public DLightSource
	{
		DUEL_DECLARE_RTTI(DAmbientLight)
	public:
		DAmbientLight(const DString& name);
		virtual LightType	getLightType();
		virtual virtual DString	getTypeName();
	};

	class DUEL_API DPointLight : public DLightSource
	{
		DUEL_DECLARE_RTTI(DPointLight)
	public:
		DPointLight(const DString& name);
		virtual LightType	getLightType();
		virtual virtual DString	getTypeName();

		void		setRadius(DReal r);
		DReal		getRadius() const;
	protected:
		DReal		mRadius;
	};

	class DUEL_API DDirectionalLight : public DLightSource
	{
		DUEL_DECLARE_RTTI(DDirectionalLight)
	public:
		DDirectionalLight(const DString& name);
		virtual LightType	getLightType();
		virtual virtual DString	getTypeName();
		// set/get direction of the light
		void			setDirection(DReal x, DReal y, DReal z);
		void			setDirection(const DVector3& dir);
		const DVector3&	getDirection() const;

		// directional light affecting area is a cylinder, the dist of 
		// the cylinder ranges from the center to the top(or buttom),
		// the radius is the radius of the top(or bottom) circle.
		void			setDistance(DReal dist);
		void			setRadius(DReal r);
		DReal			getDistance() const;
		DReal			getRadius() const;

	protected:
		DVector3		mDirection;
		DReal			mDist;
		DReal			mRadius;

	};

	class DUEL_API DSpotlight : public DLightSource
	{
		DUEL_DECLARE_RTTI(DSpotlight)
	public:
		DSpotlight(const DString& name);
		virtual LightType	getLightType();
		virtual virtual DString	getTypeName();
		// set/get direction of the light
		void			setDirection(DReal x, DReal y, DReal z);
		void			setDirection(const DVector3& dir);
		const DVector3&	getDirection() const;

		// valid when the light type is LT_SPOTLIGHT
		// set/get the spotlight properties
		void			setRange(const DRadian& innerAngle, const DRadian& outerAngle);
		void			setInnerAngle(const DRadian& innerAngle);
		void			setOuterAngle(const DRadian& outerAngle);
		const DRadian&	getInnerAngle() const;
		const DRadian&	getOuterAngle() const;
		void			setFalloff(DReal falloff);
		DReal			getFalloff() const;

	protected:
		DVector3		mDirection;

		DRadian			mSpotInner;
		DRadian			mSpotOuter;
		DReal			mSpotFalloff;
	};
}

#endif