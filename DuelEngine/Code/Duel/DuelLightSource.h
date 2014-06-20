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
		LT_Spotlight
	};

	class DUEL_API DLightSource : public DMovable
	{
	DUEL_DECLARE_RTTI(DLightSource)
	public:
		DLightSource(const DString& name);

		// set/get the light type
		void			setLightType(LightType type);
		LightType		getLightType() const;

		void			setShadowProperty(ShadowProperty sp);
		ShadowProperty	getShadowProperty() const;

		// set/get position of the light
		void			setPosition(DReal x, DReal y, DReal z);
		void			setPosition(const DVector3& pos);
		const DVector3&	getPosition() const;

		// set/get direction of the light
		void			setDirection(DReal x, DReal y, DReal z);
		void			setDirection(const DVector3& dir);
		const DVector3&	getDirection() const;
		
		// set/get the color properties of the light.
		void			setDiffuseColor(DReal red, DReal green, DReal blue);
		void			setDiffuseColor(const DColor& color);
		const DColor&	getDiffuseColor() const;
		void			setSpecularColor(DReal red, DReal green, DReal blue);
		void			setSpecularColor(const DColor& color);
		const DColor&	getSpecularColor() const;
		
		// directional light affecting area is a cylinder, the dist of 
		// the cylinder ranges from the center to the top(or buttom),
		// the radius is the radius of the top(or bottom) circle.
		void			setDirectionalDistance(DReal dist);
		void			setDirectionalRadius(DReal r);
		DReal			getDirectionalDistance() const;
		DReal			getDirectionalRadius() const;

		// set/get the attenuation property of the light
		void			setAttenuation(DReal range, DReal constant, DReal linear, DReal quadratic);
		// pass a pointer to get specified property, if param is not NULL
		void			getAttenuation(DReal* outRange, DReal* outConstant, DReal* outLinear, DReal* outQuadratic) const;

		// valid when the light type is LT_SPOTLIGHT
		// set/get the spotlight properties
		void			setSpotlightRange(const DRadian& innerAngle, const DRadian& outerAngle);
		void			setSpotlightInnerAngle(const DRadian& innerAngle);
		void			setSpotlightOuterAngle(const DRadian& outerAngle);
		const DRadian&	getSpotlightInnerAngle() const;
		const DRadian&	getSpotlightOuterAngle() const;
		void			setSpotlightFalloff(DReal falloff);
		DReal			getSpotLightFalloff() const;
		
		// scaling factor to indicate the relative power of a light.
		void			setPowerScale(DReal pow);
		DReal			getPowerScale() const;

		virtual DString getTypeName() const;


	protected:
		LightType		mType;
		ShadowProperty	mShadowProperty;
		bool			mbVisible;
		DVector3		mPos;
		DVector3		mDirection;

		DColor			mDiffuse;
		DColor			mSpecular;

		DReal			mDirectionalDist;
		DReal			mDirectionalRadius;

		DReal			mAttenuationRange;
		DReal			mAttenuationConst;
		DReal			mAttenuationLinear;
		DReal			mAttenuationQuad;

		DRadian			mSpotInner;
		DRadian			mSpotOuter;
		DReal			mSpotFalloff;
		DReal			mPowerScale;

	};

}

#endif