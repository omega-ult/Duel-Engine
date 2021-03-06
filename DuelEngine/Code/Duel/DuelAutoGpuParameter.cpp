//  [3/22/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelAutoGpuParameter.h"
#include "DuelCamera.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DAutoGpuParameterDelegate, DObject);
	DUEL_IMPLEMENT_RTTI_1(DNullAutoGpuParameterDelegate, DAutoGpuParameterDelegate);

	DNullAutoGpuParameterDelegate::DNullAutoGpuParameterDelegate()
	{
		mCamera = new DCamera("NullCamera");
		mCamera->setAspectRatio(1.0f);
		mCamera->setFixedYawAxis(true);
		mCamera->setFOV(DMath::HALF_PI/2);

		mCamera->setNearClipDistance(0.01f);
		mCamera->setFarClipDistance(1000.0f);

		mCamera->setEyePosition(0.0f, 0.0f, 0.0f );
		mCamera->lookAt(0.0f, 0.0f, 1.0f);

	}

	DNullAutoGpuParameterDelegate::~DNullAutoGpuParameterDelegate()
	{
		delete mCamera;
	}

	DMatrix4 DNullAutoGpuParameterDelegate::getViewMatrix()
	{
		return mCamera->getViewMatrix();
	}

	DMatrix4 DNullAutoGpuParameterDelegate::getProjectionMatrix()
	{
		return mCamera->getProjectionMatrix();
	}

	Duel::DMatrix4 DNullAutoGpuParameterDelegate::getInverseViewMatrix()
	{
		return mCamera->getInverseViewMatrix();
	}

	Duel::DMatrix4 DNullAutoGpuParameterDelegate::getInverseProjectionMatrix()
	{
		return mCamera->getInverseProjectionMatrix();
	}


	DAutoGpuParameter::DAutoGpuParameter()
	{
		mCurDelegate = &mNullDelegate;
	}

	DAutoGpuParameter::~DAutoGpuParameter()
	{

	}

	void DAutoGpuParameter::setParameterDelegate( DAutoGpuParameterDelegate* dele )
	{
		if (dele == NULL)
		{
			mCurDelegate = &mNullDelegate;
		}
		else
		{
			mCurDelegate = dele;
		}
	}

	DAutoGpuParameterDelegate* DAutoGpuParameter::getCurrentParameterDelegate()
	{
		return mCurDelegate;
	}

	DMatrix4 DAutoGpuParameter::getViewMatrix()
	{
		if (mCurDelegate == NULL)
		{
			return mNullDelegate.getViewMatrix();
		}
		return mCurDelegate->getViewMatrix();
	}

	DMatrix4 DAutoGpuParameter::getProjectionMatrix() 
	{
		if (mCurDelegate == NULL)
		{
			return mNullDelegate.getProjectionMatrix();
		}
		return mCurDelegate->getProjectionMatrix();
	}


	Duel::DMatrix4 DAutoGpuParameter::getInverseViewMatrix()
	{
		if (mCurDelegate == NULL)
		{
			return mNullDelegate.getInverseViewMatrix();
		}
		return mCurDelegate->getInverseViewMatrix();
	}

	Duel::DMatrix4 DAutoGpuParameter::getInverseProjectionMatrix()
	{
		if (mCurDelegate == NULL)
		{
			return mNullDelegate.getInverseProjectionMatrix();
		}
		return mCurDelegate->getInverseProjectionMatrix();
	}

}