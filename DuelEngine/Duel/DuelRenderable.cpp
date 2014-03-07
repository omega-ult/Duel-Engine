//  [1/4/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelRenderable.h"
#include "DuelAutoGpuParameter.h"
#include "DuelGpuParameter.h"
#include "DuelCamera.h"
#include "DuelGpuProgram.h"
#include "DuelRenderPass.h"
#include "DuelShaderObject.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DRenderable, DObject);


	void DRenderable::updateAutoGpuParameter(DShaderObject* so)
	{
		if (so == NULL)
		{
			return;
		}
		DAutoGpuParameter* autoParam = DAutoGpuParameter::getSingletonPtr();
		DMatrix4 w;
		DMatrix4 iw;
		bool bWCal = false;
		DMatrix4 v;
		DMatrix4 iv;
		bool bVCal = false;
		DMatrix4 p;
		DMatrix4 ip;
		bool bPCal = false;
		DMatrix4 vp;
		DMatrix4 ivp;
		bool bVPCal = false;
		DMatrix4 wv;
		DMatrix4 iwv;
		bool bWVCal = false;
		DMatrix4 wvp;
		DMatrix4 iwvp;
		bool bWVPCal = false;
	
		DGpuParameters* vsParam = so->getVertexProgramParameters();
		DGpuParameters* psParam = so->getPixelProgramParameters();
		uint64 vsMask = (vsParam == NULL) ? 0 : vsParam->getAutoParameterMask();
		uint64 psMask = (psParam == NULL) ? 0 : psParam->getAutoParameterMask();
		//////////////////////////////////////////////////////////////////////////
		// WORLD
		if (vsMask & AGPN_WorldMatrix || psMask & AGPN_WorldMatrix)
		{
			w = DMatrix4::IDENTITY;
			getWorldTransform(w);
			bWCal = true;
			if (vsMask & AGPN_WorldMatrix) vsParam->setValue("Auto_WorldMatrix", w);
			if (psMask & AGPN_WorldMatrix) psParam->setValue("Auto_WorldMatrix", w);
		}
		if (vsMask & AGPN_InvWorldMatrix || psMask & AGPN_InvWorldMatrix)
		{
			if (!bWCal)
			{
				// reuse previous result.
				w = DMatrix4::IDENTITY;
				getWorldTransform(w);
				bWCal = true;
			}
			iw = w.getInverse();
			if (vsMask & AGPN_InvWorldMatrix) vsParam->setValue("Auto_InvWorldMatrix", iw);
			if (psMask & AGPN_InvWorldMatrix) psParam->setValue("Auto_InvWorldMatrix", iw);
		}
		//////////////////////////////////////////////////////////////////////////
		// VIEW
		if (vsMask & AGPN_ViewMatrix || psMask & AGPN_ViewMatrix)
		{
			v = autoParam->getViewMatrix();
			bVCal = true;
			if (vsMask & AGPN_ViewMatrix) vsParam->setValue("Auto_ViewMatrix", v);
			if (psMask & AGPN_ViewMatrix) psParam->setValue("Auto_ViewMatrix", v);
		}
		if (vsMask & AGPN_InvViewMatrix || psMask & AGPN_InvViewMatrix)
		{
			if (!bVCal)
			{
				// reuse previous result.
				v = autoParam->getViewMatrix();
				bVCal = true;
			}
			iv = v.getInverse();
			if (vsMask & AGPN_InvViewMatrix) vsParam->setValue("Auto_InvViewMatrix", iv);
			if (psMask & AGPN_InvViewMatrix) psParam->setValue("Auto_InvViewMatrix", iv);
		}
		//////////////////////////////////////////////////////////////////////////
		// PROJ
		if (vsMask & AGPN_ProjMatrix || psMask & AGPN_ProjMatrix)
		{
			p = autoParam->getProjectionMatrix();
			bPCal = true;
			if (vsMask & AGPN_ProjMatrix) vsParam->setValue("Auto_ProjMatrix", p);
			if (psMask & AGPN_ProjMatrix) psParam->setValue("Auto_ProjMatrix", p);
		}
		if (vsMask & AGPN_InvProjMatrix || psMask & AGPN_InvProjMatrix)
		{
			if (!bPCal)
			{
				// reuse previous result.
				p = autoParam->getProjectionMatrix();
			}
			ip = p.getInverse();
			if (vsMask & AGPN_InvProjMatrix) vsParam->setValue("Auto_InvProjMatrix", ip);
			if (psMask & AGPN_InvProjMatrix) psParam->setValue("Auto_InvProjMatrix", ip);
		}
		//////////////////////////////////////////////////////////////////////////
		// VIEW_PROJ
		if (vsMask & AGPN_ViewProjMatrix || psMask & AGPN_ViewProjMatrix)
		{
			if (!bVCal)
			{
				v = autoParam->getViewMatrix();
				bVCal = true;
			}
			if (!bPCal)
			{
				p = autoParam->getProjectionMatrix();
				bPCal = true;
			}
			vp = v*p;
			bVPCal = true;
			if (vsMask & AGPN_ViewProjMatrix) vsParam->setValue("Auto_ViewProjMatrix", vp);
			if (psMask & AGPN_ViewProjMatrix) psParam->setValue("Auto_ViewProjMatrix", vp);
		}
		if (vsMask & AGPN_InvViewProjMatrix || psMask & AGPN_InvViewProjMatrix)
		{
			if (!bVPCal)
			{
				// reuse previous result.if (!bVCal)
				{
					v = autoParam->getViewMatrix();
					bVCal = true;
				}
				if (!bPCal)
				{
					p = autoParam->getProjectionMatrix();
					bPCal = true;
				}
				vp = v*p;
				bVPCal = true;
			}
			ivp = vp.getInverse();
			if (vsMask & AGPN_InvViewProjMatrix) vsParam->setValue("Auto_InvViewProjMatrix", ivp);
			if (psMask & AGPN_InvViewProjMatrix) psParam->setValue("Auto_InvViewProjMatrix", ivp);
		}
		//////////////////////////////////////////////////////////////////////////
		//  WORLD_VIEW
		if (vsMask & AGPN_WorldViewMatrix || psMask & AGPN_WorldViewMatrix)
		{
			if (!bWCal)
			{
				w = DMatrix4::IDENTITY;
				getWorldTransform(w);
				bWCal = true;
			}
			if (!bVCal)
			{
				v = autoParam->getViewMatrix();
				bVCal = true;
			}
			wv = w*v;
			bWVCal = true;
			if (vsMask & AGPN_WorldViewMatrix) vsParam->setValue("Auto_WorldViewMatrix", wv);
			if (psMask & AGPN_WorldViewMatrix) psParam->setValue("Auto_WorldViewMatrix", wv);
		}
		if (vsMask & AGPN_InvWorldViewMatrix || psMask & AGPN_InvWorldViewMatrix)
		{
			if (!bWVCal)
			{
				if (!bWCal)
				{
					w = DMatrix4::IDENTITY;
					getWorldTransform(w);
					bWCal = true;
				}
				if (!bVCal)
				{
					v = autoParam->getViewMatrix();
					bVCal = true;
				}
				wv = w*v;
				bWVCal = true;
			}
			iwv = wv.getInverse();
			if (vsMask & AGPN_InvWorldViewMatrix) vsParam->setValue("Auto_InvWorldViewMatrix", iwv);
			if (psMask & AGPN_InvWorldViewMatrix) psParam->setValue("Auto_InvWorldViewMatrix", iwv);
		}
		//////////////////////////////////////////////////////////////////////////
		// WORLD_VIEW_PROJ
		if (vsMask & AGPN_WorldViewProjMatrix || psMask & AGPN_WorldViewProjMatrix)
		{
			if (!bWCal)
			{
				w = DMatrix4::IDENTITY;
				getWorldTransform(w);
				bWCal = true;
			}
			if (!bVCal)
			{
				v = autoParam->getViewMatrix();
				bVCal = true;
			}
			if (!bPCal)
			{
				p = autoParam->getProjectionMatrix();
				bPCal = true;
			}
			wvp = w*v*p;
			bWVPCal = true;
			if (vsMask & AGPN_WorldViewProjMatrix) vsParam->setValue("Auto_WorldViewProjMatrix", wvp);
			if (psMask & AGPN_WorldViewProjMatrix) psParam->setValue("Auto_WorldViewProjMatrix", wvp);
		}
		if (vsMask & AGPN_InvWorldViewProjMatrix || psMask & AGPN_InvWorldViewProjMatrix)
		{
			if (!bWVPCal)
			{
				if (!bWCal)
				{
					w = DMatrix4::IDENTITY;
					getWorldTransform(w);
					bWCal = true;
				}
				if (!bVCal)
				{
					v = autoParam->getViewMatrix();
					bVCal = true;
				}
				if (!bPCal)
				{
					p = autoParam->getProjectionMatrix();
					bPCal = true;
				}
				wvp = w*v*p;
				bWVPCal = true;
			}
			iwvp = wvp.getInverse();
			if (vsMask & AGPN_InvWorldViewProjMatrix) vsParam->setValue("Auto_InvWorldViewProjMatrix", iwvp);
			if (psMask & AGPN_InvWorldViewProjMatrix) psParam->setValue("Auto_InvWorldViewProjMatrix", iwvp);
		}
	}

	DReal DRenderable::getSquaredViewDepthToCamera( const DCamera* cam )
	{
		if (mCacheCamera == cam )
		{
			return mCacheDepth;
		}
		mCacheCamera = cam;
		DMatrix4 t = DMatrix4::IDENTITY;
		getWorldTransform(t);
		DVector3 p;
		t.getTranslate(p);
		mCacheDepth = (p-cam->getEyePosition()).dotProduct(cam->getDirection());
		return mCacheDepth;
	}

}