//  [3/2/2014 OMEGA] created

#include "DuelCommon.h"
#include "DuelPostEffect.h"
#include "DuelShaderObject.h"
#include "DuelResourceGroupManager.h"
#include "DuelRenderResourceManager.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DPostEffectInstance, DObject);
	DUEL_IMPLEMENT_RTTI_1(DPostEffect, DObject);
	
	std::map<PostEffectParameterType, DString> initParamNameMap()
	{
		std::map<PostEffectParameterType, DString> ret;
		ret[PEPT_Color] = "color";
		ret[PEPT_Texture] = "texture";
		ret[PEPT_Int1] = "int1";
		ret[PEPT_Int2] = "int2";
		ret[PEPT_Int3] = "int3";
		ret[PEPT_Int4] = "int4";
		ret[PEPT_Float1] = "float1";
		ret[PEPT_Float2] = "float2";
		ret[PEPT_Float3] = "float3";
		ret[PEPT_Float4] = "float4";
		return ret;
	}

	std::map<PostEffectParameterType, DString> DPostEffectParameter::msParamNameMap = initParamNameMap();

	bool DPostEffectParameter::isInt()
	{
		return DPostEffectParameter::isInt(paramType);
	}

	bool DPostEffectParameter::isInt( PostEffectParameterType type )
	{
		switch (type)
		{
		case PEPT_Int1:
		case PEPT_Int2:
		case PEPT_Int3:
		case PEPT_Int4:
			return true;
		}
		return false;
	}

	bool DPostEffectParameter::isFloat()
	{
		return DPostEffectParameter::isFloat(paramType);
	}

	bool DPostEffectParameter::isFloat( PostEffectParameterType type )
	{
		switch (type)
		{
		case PEPT_Float1:
		case PEPT_Float2:
		case PEPT_Float3:
		case PEPT_Float4:
			return true;
		}
		return false;
	}

	bool DPostEffectParameter::isColor()
	{
		return DPostEffectParameter::isColor(paramType);
	}

	bool DPostEffectParameter::isColor( PostEffectParameterType type )
	{
		return type == PEPT_Color;
	}

	bool DPostEffectParameter::isTexture()
	{
		return DPostEffectParameter::isTexture(paramType);
	}

	bool DPostEffectParameter::isTexture( PostEffectParameterType type )
	{
		return type == PEPT_Color;
	}

	Duel::uint32 DPostEffectParameter::getElementSize( PostEffectParameterType type )
	{
		switch (type)
		{
		case Duel::PEPT_Texture:
			return 0;
		case Duel::PEPT_Int1:
		case Duel::PEPT_Float1:
			return 1;
		case Duel::PEPT_Int2:
		case Duel::PEPT_Float2:
			return 2;
		case Duel::PEPT_Int3:
		case Duel::PEPT_Float3:
			return 3;
		case Duel::PEPT_Color:
		case Duel::PEPT_Int4:
		case Duel::PEPT_Float4:
			return 4;
		default:
			break;
		}
		return 0;
	}

	const DString& DPostEffectParameter::getParameterTypeName()
	{
		return DPostEffectParameter::getParameterTypeName(paramType);
	}

	const DString& DPostEffectParameter::getParameterTypeName( PostEffectParameterType type )
	{
		std::map<PostEffectParameterType, DString>::iterator i = msParamNameMap.find(type);
		if (i != msParamNameMap.end())
		{
			return i->second;
		}
		return DStringTool::BLANK;
	}


	DPostEffectInstance::DPostEffectInstance( DPostEffect* parent ) :
		mParent(parent)
	{

	}

	void DPostEffectInstance::addParameter( const DString& name, PostEffectParameterType type, const DString& gpuParam, const DString& gpuProgram, bool openToUser /*= false*/ )
	{

		DPostEffectParameter param;
		param.paramType = type;
		param.paramName = name;
		param.elemSize = DPostEffectParameter::getElementSize(type);
		param.bUserAccess = openToUser;
		param.physicalIndex = 0;
		param.targetGpuParam = gpuParam;
		param.targetGpuProgram = gpuProgram;
		if (param.isColor() || param.isFloat())
		{
			param.physicalIndex = mFloatConstants.size();
			mFloatConstants.insert(mFloatConstants.end(), param.elemSize, 0.0f);
		}
		else if (param.isInt())
		{
			param.physicalIndex = mIntConstants.size();
			mIntConstants.insert(mIntConstants.end(), param.elemSize, 0);
		}
		mParamMap[name] = param;
	}

	void DPostEffectInstance::removeParameter( const DString& name )
	{
		ParameterMap::iterator i = mParamMap.find(name);
		if (i != mParamMap.end())
		{
			mParamMap.erase(i);
		}

	}

	void DPostEffectInstance::removeAllParameters()
	{
		mParamMap.clear();
	}

	void DPostEffectInstance::setValue( const DString& name, float val )
	{
		ParameterMap::iterator i = mParamMap.find(name);
		if (i != mParamMap.end())
		{
			mFloatConstants[i->second.physicalIndex] = val;
		}
	}

	void DPostEffectInstance::setValue( const DString& name, float* val, uint32 count )
	{
		if (!(count > 0 && count < 5))
		{
			return;
		}
		ParameterMap::iterator i = mParamMap.find(name);
		if (i != mParamMap.end())
		{
			assert(i->second.physicalIndex + sizeof(float) * count <= mFloatConstants.size());
			memcpy(&mFloatConstants[i->second.physicalIndex], val, count * sizeof(float));
		}
	}

	void DPostEffectInstance::setValue( const DString& name, int val )
	{
		ParameterMap::iterator i = mParamMap.find(name);
		if (i != mParamMap.end())
		{
			mIntConstants[i->second.physicalIndex] = val;
		}
	}

	void DPostEffectInstance::setValue( const DString& name, int* val, uint32 count )
	{
		ParameterMap::iterator i = mParamMap.find(name);
		if (i != mParamMap.end())
		{
			assert(i->second.physicalIndex + i->second.elemSize * count <= mIntConstants.size());
			memcpy(&mIntConstants[i->second.physicalIndex], val, count * sizeof(int));
		}
	}

	void DPostEffectInstance::setValue( const DString& name, const DVector2& vec )
	{
		ParameterMap::iterator i = mParamMap.find(name);
		if (i != mParamMap.end())
		{
			mFloatConstants[i->second.physicalIndex] = vec[0];
			mFloatConstants[i->second.physicalIndex+1] = vec[1];
		}
	}

	void DPostEffectInstance::setValue( const DString& name, const DVector3& vec )
	{
		ParameterMap::iterator i = mParamMap.find(name);
		if (i != mParamMap.end())
		{
			mFloatConstants[i->second.physicalIndex] = vec[0];
			mFloatConstants[i->second.physicalIndex+1] = vec[1];
			mFloatConstants[i->second.physicalIndex+2] = vec[2];
		}
	}

	void DPostEffectInstance::setValue( const DString& name, const DVector4& vec )
	{
		ParameterMap::iterator i = mParamMap.find(name);
		if (i != mParamMap.end())
		{
			mFloatConstants[i->second.physicalIndex] = vec[0];
			mFloatConstants[i->second.physicalIndex+1] = vec[1];
			mFloatConstants[i->second.physicalIndex+2] = vec[2];
			mFloatConstants[i->second.physicalIndex+3] = vec[3];
		}
	}

	void DPostEffectInstance::setValue( const DString& name, const DColor& val )
	{
		ParameterMap::iterator i = mParamMap.find(name);
		if (i != mParamMap.end())
		{
			memcpy(&mFloatConstants[i->second.physicalIndex], val.ptr(), 4 * sizeof(float));
		}
	}

	void DPostEffectInstance::setValue( const DString& paramName, TextureConstant tex )
	{
		TextureConstantCache texCache;
		ParameterMap::iterator i = mParamMap.find(paramName);
		if (i != mParamMap.end())
		{
			DGpuTextureConstantPtr c;
			DResourcePtr texRes = DResourceGroupManager::getSingleton().getResouceManager("Texture")->getResource(tex.first, tex.second);
			if (texRes != NULL && texRes->isLoaded())
			{
				c = texRes->getAs<DTexture>()->getGpuTexutureConstant();	
				texCache.second = c;
			}
			texCache.first = tex;
			mTextureMap[paramName] = texCache;
		}
	}

	float* DPostEffectInstance::getFloatValuePtr( uint32 physicalIndex )
	{
		return &mFloatConstants[physicalIndex];
	}

	int32* DPostEffectInstance::getIntValuePtr( uint32 physicalIndex )
	{
		return &mIntConstants[physicalIndex];
	}

	DPostEffectInstance::TextureConstantCache DPostEffectInstance::getTextureValue( const DString& paramName )
	{
		TextureConstantMap::iterator i = mTextureMap.find(paramName);
		if (i != mTextureMap.end())
		{
			return i->second;
		}
		return TextureConstantCache();
	}

	void DPostEffectInstance::copyTo( DPostEffectInstance* inst )
	{
		if (inst != NULL)
		{
			*inst = *this;
		}
	}


	DPostEffect::DPostEffect( const DString& name ) :
		mName(name)
	{
		mRenderLayout = DRenderResourceManager::getSingleton().createRenderLayout();
		// a quad.
		float data[] =
		{ 
			/*vert*/ -1.0f, 1.0f,
			/*vert*/ 1.0f,  1.0f,
			/*vert*/ 1.0f, -1.0f,
			/*vert*/ -1.0f,-1.0f
		};
		DVertexDeclaration vd;
		vd.addElement(0, 0, VET_Float2, VES_Position);
		DVertexStreams vs;
		DVertexBufferPtr vb = DRenderResourceManager::getSingleton().createVetexBuffer(
			sizeof(float) * 2, 4, HBU_Static, false);
		vb->writeData(0,  vb->getSize(), data, true);
		vs.setStream(0, vb);
		int16 idata[] =
		{
			0, 3, 2,  0, 2, 1
		};
		DIndexBufferPtr ib = DRenderResourceManager::getSingleton().createIndexBuffer(
			IT_16Bit, 6, HBU_Static, false);
		ib->writeData(0, ib->getSize(), idata, true);

		mRenderLayout->setIndexData(DIndexData(ib));
		mRenderLayout->setVertexData(DVertexData(vs, vd));
		mRenderLayout->setTopologyType(PT_TriangleList);
	}

	DPostEffect::~DPostEffect()
	{

	}

	const DString& DPostEffect::getPostEffectName()
	{
		return mName;
	}

	Duel::DPostEffectInstancePtr DPostEffect::createInstance()
	{
		DPostEffectInstance* retInstance = new DPostEffectInstance(this);
		*retInstance = *mPersistInstance;
		return DPostEffectInstancePtr(retInstance);
	}

	void DPostEffect::updateGpuParameter( DPostEffectInstance* inst, DShaderObject* so )
	{
		if (!so->isValid())
		{
			return;
		}
		// first build the param list, for vs and ps.
		typedef std::vector<DPostEffectParameter>	ParamerterList;
		ParamerterList vsParams;
		ParamerterList psParams;
		DPostEffectInstance::ParameterIterator pI = inst->getParameterIterator();
		while (pI.hasMoreElements())
		{
			DPostEffectParameter& param = pI.getNext();
			if (param.targetGpuProgram == so->getVertexProgram()->getName())
			{
				vsParams.push_back(param);
			}
			else if (param.targetGpuProgram == so->getVertexProgram()->getName())
			{
				psParams.push_back(param);
			}
		}

		// for vertex shader
		DGpuParameters* gpuParam = so->getVertexProgramParameters();
		ParamerterList::iterator i, iend = vsParams.end();
		for (i = vsParams.begin(); i != iend; ++i)
		{
			DPostEffectParameter p = *i;
			if (p.isFloat())
			{
				gpuParam->setValue(p.targetGpuParam, inst->getFloatValuePtr(p.physicalIndex), p.elemSize);
			}
			else if (p.isInt())
			{
				gpuParam->setValue(p.targetGpuParam, inst->getIntValuePtr(p.physicalIndex), p.elemSize);
			}
			else if (p.isTexture())
			{
				DPostEffectInstance::TextureConstantCache c = inst->getTextureValue(p.paramName);
				gpuParam->setValue(p.targetGpuParam, c.second);
			}
		}

		// for pixel shader
		gpuParam = so->getPixelProgramParameters();
		iend = psParams.end();
		for (i = psParams.begin(); i != iend; ++i)
		{
			DPostEffectParameter p = *i;
			if (p.isFloat())
			{
				gpuParam->setValue(p.targetGpuParam, inst->getFloatValuePtr(p.physicalIndex), p.elemSize);
			}
			else if (p.isInt())
			{
				gpuParam->setValue(p.targetGpuParam, inst->getIntValuePtr(p.physicalIndex), p.elemSize);
			}
			else if (p.isTexture())
			{
				DPostEffectInstance::TextureConstantCache c = inst->getTextureValue(p.paramName);
				gpuParam->setValue(p.targetGpuParam, c.second);
			}
		}
	}


}