//  [12/6/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelLightSource.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DLightSource, DMovable);
	DUEL_IMPLEMENT_RTTI_1(DAmbientLight, DLightSource);
	DUEL_IMPLEMENT_RTTI_1(DPointLight, DLightSource);
	DUEL_IMPLEMENT_RTTI_1(DDirectionalLight, DLightSource);
	DUEL_IMPLEMENT_RTTI_1(DSpotlight, DLightSource);

	DLightSource::DLightSource( const DString& name ) :
		DMovable(name),
		mShadowProperty(SP_None),
		mPos(DVector3::ZERO),
		mDiffuseColor(DColor::WHITE),
		mSpecularColor(DColor::BLACK)
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


	void DLightSource::setDiffuseColor( DReal red, DReal green, DReal blue )
	{
		mDiffuseColor.r = red;
		mDiffuseColor.g = green;
		mDiffuseColor.b = blue;
	}

	void DLightSource::setDiffuseColor( const DColor& color )
	{
		mDiffuseColor = color;
	}

	const DColor& DLightSource::getDiffuseColor() const
	{
		return mDiffuseColor;
	}

	void DLightSource::setSpecularColor( DReal red, DReal green, DReal blue )
	{
		mSpecularColor.r = red;
		mSpecularColor.g = green;
		mSpecularColor.b = blue;
	}

	void DLightSource::setSpecularColor( const DColor& color )
	{
		mSpecularColor = color;
	}

	const DColor& DLightSource::getSpecularColor() const
	{
		return mSpecularColor;
	}
	

	Duel::ShadowProperty DLightSource::getShadowProperty() const
	{
		return mShadowProperty;
	}

	void DLightSource::setShadowProperty( ShadowProperty sp )
	{
		mShadowProperty = sp;
	}


	DAmbientLight::DAmbientLight( const DString& name ) :
		DLightSource(name)
	{

	}

	Duel::LightType DAmbientLight::getLightType()
	{
		return LT_Ambient;
	}

	Duel::DString DAmbientLight::getTypeName()
	{
		return "AmbientLight";
	}




	DPointLight::DPointLight( const DString& name ) : 
		DLightSource(name),
		mRadius(1.0f)
	{

	}

	Duel::LightType DPointLight::getLightType()
	{
		return LT_Point;
	}

	Duel::DString DPointLight::getTypeName()
	{
		return "PointLight";
	}

	void DPointLight::setRadius( DReal r )
	{
		mRadius = r;
	}

	Duel::DReal DPointLight::getRadius() const
	{
		return mRadius;
	}


	DDirectionalLight::DDirectionalLight( const DString& name ) :
		DLightSource(name),
		mDirection(DVector3::UNIT_Z),
		mDist(1.0f),
		mRadius(1.0f)
	{

	}

	Duel::LightType DDirectionalLight::getLightType()
	{
		return LT_Directional;
	}

	Duel::DString DDirectionalLight::getTypeName()
	{
		return "DirectionalLight";
	}

	void DDirectionalLight::setDirection( DReal x, DReal y, DReal z )
	{
		mDirection = DVector3(x,y,z).normalize();
	}

	void DDirectionalLight::setDirection( const DVector3& dir )
	{
		mDirection = dir;
		mDirection.normalize();
	}

	const DVector3& DDirectionalLight::getDirection() const
	{
		return mDirection;
	}

	void DDirectionalLight::setDistance( DReal dist )
	{
		mDist = dist;
	}

	void DDirectionalLight::setRadius( DReal r )
	{
		mRadius = r;
	}

	Duel::DReal DDirectionalLight::getDistance() const
	{
		return mDist;
	}

	Duel::DReal DDirectionalLight::getRadius() const
	{
		return mRadius;
	}

	DSpotlight::DSpotlight( const DString& name ) :
		DLightSource(name),
		mDirection(DVector3::UNIT_Z),
		mSpotInner(DMath::HALF_PI/2),
		mSpotOuter(DMath::HALF_PI),
		mSpotFalloff(1.0f)
	{

	}

	Duel::LightType DSpotlight::getLightType()
	{
		return LT_Spotlight;
	}

	Duel::DString DSpotlight::getTypeName()
	{
		return "Spotlight";
	}

	void DSpotlight::setDirection( DReal x, DReal y, DReal z )
	{
		mDirection = DVector3(x,y,z).normalize();
	}

	void DSpotlight::setDirection( const DVector3& dir )
	{
		mDirection = dir;
		mDirection.normalize();
	}

	const DVector3& DSpotlight::getDirection() const
	{
		return mDirection;
	}

	void DSpotlight::setRange( const DRadian& innerAngle, const DRadian& outerAngle )
	{
		mSpotInner = innerAngle <= outerAngle ? innerAngle : outerAngle;
		mSpotOuter = innerAngle < outerAngle ? outerAngle : innerAngle;
	}

	void DSpotlight::setInnerAngle( const DRadian& innerAngle )
	{
		mSpotInner = innerAngle > mSpotOuter ? mSpotOuter : innerAngle;
	}

	void DSpotlight::setOuterAngle( const DRadian& outerAngle )
	{
		mSpotOuter = outerAngle < mSpotInner ? mSpotInner : outerAngle;
	}

	const DRadian& DSpotlight::getInnerAngle() const
	{
		return mSpotInner;
	}

	const DRadian& DSpotlight::getOuterAngle() const
	{
		return mSpotOuter;
	}

	void DSpotlight::setFalloff( DReal falloff )
	{
		mSpotFalloff = falloff;
	}

	Duel::DReal DSpotlight::getFalloff() const
	{
		return mSpotFalloff;
	}













}