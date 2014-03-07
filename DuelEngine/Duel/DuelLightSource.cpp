//  [12/6/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelLightSource.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DLightSource, DMovable);


	DLightSource::DLightSource( const DString& name ) :
		DMovable(name),
		mType(LT_Point),
		mPos(DVector3::ZERO),
		mDiffuse(DColor::WHITE),
		mSpecular(DColor::BLACK),
		mDirection(DVector3::UNIT_Z),
		mSpotOuter(DDegree(40.0f)),
		mSpotInner(DDegree(30.0f)),
		mSpotFalloff(1.0f),
		mAttenuationRange(100000),
		mAttenuationConst(1.0f),
		mAttenuationLinear(0.0f),
		mAttenuationQuad(0.0f),
		mPowerScale(1.0f)
	{
		mBoundingBox.setFinite();
		mBoundingBox.setExtent(-0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f);
	}

	void DLightSource::setPosition( DReal x, DReal y, DReal z )
	{
		mPos.x = x;
		mPos.y = y;
		mPos.z = z;
	}

	void DLightSource::setPosition( const DVector3& pos )
	{
		mPos = pos;
	}

	const DVector3& DLightSource::getPosition() const
	{
		return mPos;
	}

	void DLightSource::setDirection( DReal x, DReal y, DReal z )
	{
		mDirection.x = x;
		mDirection.y = y;
		mDirection.z = z;
	}

	void DLightSource::setDirection( const DVector3& dir )
	{
		mDirection = dir;
	}

	const DVector3& DLightSource::getDirection() const
	{
		return mDirection;
	}

	void DLightSource::setDiffuseColor( DReal red, DReal green, DReal blue )
	{
		mDiffuse.r = red;
		mDiffuse.g = green;
		mDiffuse.b = blue;
	}

	void DLightSource::setDiffuseColor( const DColor& color )
	{
		mDiffuse = color;
	}

	const DColor& DLightSource::getDiffuseColor() const
	{
		return mDiffuse;
	}

	void DLightSource::setSpecularColor( DReal red, DReal green, DReal blue )
	{
		mSpecular.r = red;
		mSpecular.g = green;
		mSpecular.b = blue;
	}

	void DLightSource::setSpecularColor( const DColor& color )
	{
		mSpecular = color;
	}

	const DColor& DLightSource::getSpecularColor() const
	{
		return mSpecular;
	}

	void DLightSource::setAttenuation( DReal range, DReal constant, DReal linear, DReal quadratic )
	{
		mAttenuationRange = range;
		mAttenuationConst = constant;
		mAttenuationLinear = linear;
		mAttenuationQuad = quadratic;
	}

	void DLightSource::getAttenuation( DReal* outRange, DReal* outConstant, DReal* outLinear, DReal* outQuadratic ) const
	{
		if (outRange != NULL)
		{
			*outRange = mAttenuationRange;
		}
		if (outConstant != NULL)
		{
			*outConstant = mAttenuationConst;
		}
		if (outLinear != NULL)
		{
			*outLinear = mAttenuationLinear;
		}
		if (outQuadratic != NULL)
		{
			*outQuadratic = mAttenuationQuad;
		}
	}

	void DLightSource::setSpotlightRange( const DRadian& innerAngle, const DRadian& outerAngle )
	{
		mSpotInner = innerAngle;
		mSpotOuter = outerAngle;
	}

	void DLightSource::setSpotlightInnerAngle( const DRadian& innerAngle )
	{
		mSpotInner = innerAngle;
	}

	void DLightSource::setSpotlightOuterAngle( const DRadian& outerAngle )
	{
		mSpotOuter = outerAngle;
	}

	const DRadian& DLightSource::getSpotlightInnerAngle() const
	{
		return mSpotInner;
	}

	const DRadian& DLightSource::getSpotlightOuterAngle() const
	{
		return mSpotOuter;
	}

	void DLightSource::setSpotlightFalloff( DReal falloff )
	{
		mSpotFalloff = falloff;
	}

	DReal DLightSource::getSpotLightFalloff() const
	{
		return mSpotFalloff;
	}

	void DLightSource::setPowerScale( DReal pow )
	{
		mPowerScale = pow;
	}

	DReal DLightSource::getPowerScale() const
	{
		return mPowerScale;
	}

	Duel::DString DLightSource::getTypeName() const
	{
		return "LightSource";
	}

}