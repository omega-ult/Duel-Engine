//  [5/23/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelColor.h"
#include "DuelRenderPass.h"
#include "DuelGpuProgram.h"
#include "DuelGpuParameter.h"
#include "DuelResourceGroupManager.h"
#include "DuelTexture.h"
#include "DuelMaterial.h"
#include "DuelMaterialManager.h"
#include "DuelShaderObject.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DMaterialInstance, DObject);
	DUEL_IMPLEMENT_RTTI_1(DMaterial, DObject);

	std::map<MaterialParameterType, DString> initParamNameMap()
	{
		std::map<MaterialParameterType, DString> ret;
		ret[MPT_Bool] = "bool";
		ret[MPT_Color] = "color";
		ret[MPT_Texture] = "texture";
		ret[MPT_Sampler] = "sampler";
		ret[MPT_Int1] = "int1";
		ret[MPT_Int2] = "int2";
		ret[MPT_Int3] = "int3";
		ret[MPT_Int4] = "int4";
		ret[MPT_Float1] = "float1";
		ret[MPT_Float2] = "float2";
		ret[MPT_Float3] = "float3";
		ret[MPT_Float4] = "float4";
		ret[MPT_Matrix2x2] = "matrix2x2";
		ret[MPT_Matrix2x3] = "matrix2x3";
		ret[MPT_Matrix2x4] = "matrix2x4";
		ret[MPT_Matrix3x2] = "matrix3x2";
		ret[MPT_Matrix3x3] = "matrix3x3";
		ret[MPT_Matrix3x4] = "matrix3x4";
		ret[MPT_Matrix4x2] = "matrix4x2";
		ret[MPT_Matrix4x3] = "matrix4x3";
		ret[MPT_Matrix4x4] = "matrix4x4";
		return ret;
	}

	std::map<MaterialParameterType, DString>	DMaterialParameter::msParamNameMap = initParamNameMap();

	bool DMaterialParameter::isBool()
	{
		return isBool(paramType);
	}

	bool DMaterialParameter::isBool( MaterialParameterType type )
	{
		return type == MPT_Bool;
	}

	bool DMaterialParameter::isInt()
	{
		return DMaterialParameter::isInt(paramType);
	}

	bool DMaterialParameter::isInt( MaterialParameterType type )
	{
		switch (type)
		{
		case MPT_Int1:
		case MPT_Int2:
		case MPT_Int3:
		case MPT_Int4:
			return true;
		}
		return false;
	}

	bool DMaterialParameter::isFloat()
	{
		return DMaterialParameter::isFloat(paramType);
	}

	bool DMaterialParameter::isFloat( MaterialParameterType type )
	{
		switch (type)
		{
		case MPT_Float1:
		case MPT_Float2:
		case MPT_Float3:
		case MPT_Float4:
			return true;
		}
		return false;
	}

	bool DMaterialParameter::isMatrix()
	{
		return DMaterialParameter::isMatrix(paramType);
	}

	bool DMaterialParameter::isMatrix( MaterialParameterType type )
	{
		switch (type)
		{
		case MPT_Matrix2x2:
		case MPT_Matrix2x3:
		case MPT_Matrix2x4:
		case MPT_Matrix3x2:
		case MPT_Matrix3x3:
		case MPT_Matrix3x4:
		case MPT_Matrix4x2:
		case MPT_Matrix4x3:
		case MPT_Matrix4x4:
			return true;
		}
		return false;
	}

	bool DMaterialParameter::isColor()
	{
		return DMaterialParameter::isColor(paramType);
	}

	bool DMaterialParameter::isColor( MaterialParameterType type )
	{
		return type == MPT_Color;
	}

	bool DMaterialParameter::isTexture()
	{
		return DMaterialParameter::isTexture(paramType);
	}

	bool DMaterialParameter::isTexture( MaterialParameterType type )
	{
		return type == MPT_Texture;
	}

	bool DMaterialParameter::isSampler()
	{
		return isSampler(paramType);
	}

	bool DMaterialParameter::isSampler( MaterialParameterType type )
	{
		return type == MPT_Sampler;
	}



	Duel::uint32 DMaterialParameter::getElementSize( MaterialParameterType type )
	{
		switch (type)
		{
		case Duel::MPT_Texture:
		case Duel::MPT_Sampler:
			return 0;
		case Duel::MPT_Bool:
		case Duel::MPT_Int1:
		case Duel::MPT_Float1:
			return 1;
		case Duel::MPT_Int2:
		case Duel::MPT_Float2:
			return 2;
		case Duel::MPT_Int3:
		case Duel::MPT_Float3:
			return 3;
		case Duel::MPT_Color:
		case Duel::MPT_Int4:
		case Duel::MPT_Float4:
		case Duel::MPT_Matrix2x2:
			return 4;
		case Duel::MPT_Matrix2x3:
		case Duel::MPT_Matrix3x2:
			return 6;
		case Duel::MPT_Matrix2x4:
		case Duel::MPT_Matrix4x2:
			return 8;
		case Duel::MPT_Matrix3x3:
			return 9;
		case Duel::MPT_Matrix3x4:
		case Duel::MPT_Matrix4x3:
			return 12;
		case Duel::MPT_Matrix4x4:
			return 16;
		default:
			break;
		}
		return 0;
	}

	const DString& DMaterialParameter::getParameterTypeName()
	{
		return DMaterialParameter::getParameterTypeName(paramType);
	}

	const DString& DMaterialParameter::getParameterTypeName( MaterialParameterType type )
	{
		std::map<MaterialParameterType, DString>::iterator i = msParamNameMap.find(type);
		if (i != msParamNameMap.end())
		{
			return i->second;
		}
		return DStringTool::BLANK;
	}

	DMaterialInstance::DMaterialInstance( DMaterial* parent ) :
		mParent(parent)
	{

	}

	void DMaterialInstance::updateGpuParameter( DShaderObject* so )
	{
		mParent->updateGpuParameter(this, so);
	}



	void DMaterialInstance::addParameter( const DString& name, const DString& gpuProgram, const DString& gpuParam, 
		MaterialParameterType type,	uint32 arraySize /*= 1*/, bool openToUser /*= false*/ )
	{
		DMaterialParameter param;
		param.paramType = type;
		param.paramName = name;
		param.arraySize = arraySize == 0 ? 1 : arraySize;
		param.elemSize = DMaterialParameter::getElementSize(type);
		param.bUserAccess = openToUser;
		param.physicalIndex = 0;
		param.targetGpuParam = gpuParam;
		param.targetGpuProgram = gpuProgram;
		if (param.isColor() || param.isFloat() || param.isMatrix())
		{
			param.physicalIndex = mFloatConstants.size();
			mFloatConstants.insert(mFloatConstants.end(), param.arraySize * param.elemSize, 0.0f);
		}
		else if (param.isInt())
		{
			param.physicalIndex = mIntConstants.size();
			mIntConstants.insert(mIntConstants.end(), param.arraySize * param.elemSize, 0);
		}
		else if (param.isBool())
		{
			param.physicalIndex = mBoolConstants.size();
			mBoolConstants.insert(mBoolConstants.end(), param.arraySize * param.elemSize, 0);
		}
		mParamMap[name] = param;
	}

	void DMaterialInstance::removeParameter( const DString& name )
	{
		ParameterMap::iterator i = mParamMap.find(name);
		if (i != mParamMap.end())
		{
			mParamMap.erase(i);
		}
	}

	void DMaterialInstance::removeAllParameters( void )
	{
		mParamMap.clear();
	}

	float* DMaterialInstance::getFloatValuePtr( uint32 physicalIndex )
	{
		return &mFloatConstants[physicalIndex];
	}

	int32* DMaterialInstance::getIntValuePtr( uint32 physicalIndex )
	{
		return &mIntConstants[physicalIndex];
	}

	int32* DMaterialInstance::getBoolValuePtr( uint32 physicalIndex )
	{
		return &mBoolConstants[physicalIndex];
	}

	DRenderTechnique* DMaterialInstance::getRenderTechnique( uint32 stage, DCamera* cam, LightIterator li )
	{
		return mParent->getRenderTechnique(stage, cam , li);
	}

	void DMaterialInstance::setValue( const DString& name, bool val )
	{
		ParameterMap::iterator i = mParamMap.find(name);
		if (i != mParamMap.end())
		{
			mBoolConstants[i->second.physicalIndex] = val ? 1 : 0;
		}
	}


	void DMaterialInstance::setValue( const DString& name, float val )
	{
		ParameterMap::iterator i = mParamMap.find(name);
		if (i != mParamMap.end())
		{
			mFloatConstants[i->second.physicalIndex] = val;
		}
	}

	void DMaterialInstance::setValue( const DString& name, float* val, uint32 count )
	{
		ParameterMap::iterator i = mParamMap.find(name);
		if (i != mParamMap.end())
		{
			assert(i->second.physicalIndex + sizeof(float) * count <= mFloatConstants.size());
			memcpy(&mFloatConstants[i->second.physicalIndex], val, count * sizeof(float));
		}

	}

	void DMaterialInstance::setValue( const DString& paramName, TextureConstant tex )
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

	void DMaterialInstance::setValue( const DString& paramName, DTextureSamplerObjectPtr samp )
	{
		ParameterMap::iterator i = mParamMap.find(paramName);
		if (i != mParamMap.end())
		{
			mSamplerMap.insert(SamplerConstantMap::value_type(paramName, samp));
		}
		
	}


	void DMaterialInstance::setValue( const DString& name, int val )
	{
		ParameterMap::iterator i = mParamMap.find(name);
		if (i != mParamMap.end())
		{
			if (i->second.isInt())
			{
				mIntConstants[i->second.physicalIndex] = val;
			}
			else if (i->second.isBool())
			{
				mBoolConstants[i->second.physicalIndex] = val;
			}
		}
	}

	void DMaterialInstance::setValue( const DString& name, int* val, uint32 count )
	{
		ParameterMap::iterator i = mParamMap.find(name);
		if (i != mParamMap.end())
		{
			if (i->second.isInt())
			{		
				assert(i->second.physicalIndex + i->second.elemSize * count <= mIntConstants.size());
				memcpy(&mIntConstants[i->second.physicalIndex], val, count * sizeof(int));
			}
			else if (i->second.isBool())
			{
				assert(i->second.physicalIndex + i->second.elemSize * count <= mBoolConstants.size());
				memcpy(&mBoolConstants[i->second.physicalIndex], val, count * sizeof(int));
			}
		}
	}

	void DMaterialInstance::setValue( const DString& name, const DVector4& vec )
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

	void DMaterialInstance::setValue( const DString& name, const DMatrix4& mat )
	{
		ParameterMap::iterator i = mParamMap.find(name);
		if (i != mParamMap.end())
		{
			memcpy(&mFloatConstants[i->second.physicalIndex], mat[0], i->second.elemSize * sizeof(float));
		}
	}

	void DMaterialInstance::setValue( const DString& name, DMatrix4* mat, uint32 count )
	{
		ParameterMap::iterator i = mParamMap.find(name);
		if (i != mParamMap.end())
		{
			assert(i->second.physicalIndex + i->second.elemSize * count <= mFloatConstants.size());
			for (uint32 c = 0; c < count; ++c)
			{
				memcpy(&mFloatConstants[i->second.physicalIndex + i->second.elemSize * c * sizeof(float)], mat[c][0], i->second.elemSize * sizeof(float));
			}
		}		
	}

	void DMaterialInstance::setValue( const DString& name, const DColor& val )
	{
		ParameterMap::iterator i = mParamMap.find(name);
		if (i != mParamMap.end())
		{
			memcpy(&mFloatConstants[i->second.physicalIndex], val.ptr(), 4 * sizeof(float));
		}
	}

	void DMaterialInstance::setValue( const DString& name, const DVector2& m1_, const DVector2& m2_ )
	{
		float tmp[4];
		tmp[0] = m1_.x; tmp[1] = m1_.y;
		tmp[2] = m2_.x; tmp[3] = m2_.y;
		setValue(name, tmp, 4);
	}

	void DMaterialInstance::setValue( const DString& name, const DVector2& m1_, const DVector2& m2_, const DVector2& m3_ )
	{
		float tmp[6];
		tmp[0] = m1_.x; tmp[1] = m1_.y;
		tmp[2] = m2_.x; tmp[3] = m2_.y;
		tmp[4] = m3_.x; tmp[5] = m3_.y;
		setValue(name, tmp, 6);
	}

	void DMaterialInstance::setValue( const DString& name, const DVector2& m1_, const DVector2& m2_, const DVector2& m3_, const DVector2& m4_ )
	{
		float tmp[8];
		tmp[0] = m1_.x; tmp[1] = m1_.y;
		tmp[2] = m2_.x; tmp[3] = m2_.y;
		tmp[4] = m3_.x; tmp[5] = m3_.y;
		tmp[6] = m3_.x; tmp[7] = m3_.y;
		setValue(name, tmp, 8);
	}

	void DMaterialInstance::setValue( const DString& name, const DVector3& m1_, const DVector3& m2_ )
	{
		float tmp[6];
		tmp[0] = m1_.x; tmp[1] = m1_.y; tmp[2] = m1_.z;
		tmp[3] = m2_.x; tmp[4] = m2_.y; tmp[5] = m2_.z;
		setValue(name, tmp, 6);
	}

	void DMaterialInstance::setValue( const DString& name, const DMatrix3& mat )
	{
		float tmp[9];
		tmp[0] = mat[0][0]; tmp[1] = mat[0][1]; tmp[2] = mat[0][2]; 
		tmp[3] = mat[1][0]; tmp[4] = mat[1][1]; tmp[5] = mat[1][2]; 
		tmp[6] = mat[2][0]; tmp[7] = mat[2][1]; tmp[8] = mat[2][2]; 
		setValue(name, tmp, 9);
	}

	void DMaterialInstance::setValue( const DString& name, const DVector3& m1_, const DVector3& m2_, const DVector3& m3_, const DVector3& m4_ )
	{
		float tmp[12];
		tmp[0] = m1_.x; tmp[1] = m1_.y; tmp[2] = m1_.z;
		tmp[3] = m2_.x; tmp[4] = m2_.y; tmp[5] = m2_.z;
		tmp[6] = m3_.x; tmp[7] = m3_.y; tmp[8] = m3_.z;
		tmp[9] = m4_.x; tmp[10] = m4_.y; tmp[11] = m4_.z;
		setValue(name, tmp, 12);
	}

	void DMaterialInstance::setValue( const DString& name, const DVector4& m1_, const DVector4& m2_ )
	{
		float tmp[8];
		tmp[0] = m1_.x; tmp[1] = m1_.y; tmp[2] = m1_.z; tmp[3] = m1_.w;
		tmp[4] = m2_.x; tmp[5] = m2_.y; tmp[6] = m2_.z; tmp[7] = m2_.w;
		setValue(name, tmp, 8);
	}

	void DMaterialInstance::setValue( const DString& name, const DVector4& m1_, const DVector4& m2_, const DVector4& m3_ )
	{
		float tmp[12];
		tmp[0] = m1_.x; tmp[1] = m1_.y; tmp[2] = m1_.z; tmp[3] = m1_.w;
		tmp[4] = m2_.x; tmp[5] = m2_.y; tmp[6] = m2_.z; tmp[7] = m2_.w;
		tmp[8] = m3_.x; tmp[9] = m3_.y; tmp[10] = m3_.z; tmp[11] = m3_.w;
		setValue(name, tmp, 12);
	}

	DMaterialInstance::TextureConstantCache DMaterialInstance::getTextureValue( const DString& paramName )
	{
		TextureConstantCacheMap::iterator i = mTextureMap.find(paramName);
		if (i != mTextureMap.end())
		{
			return i->second;
		}
		TextureConstantCache ret;
		return ret;
	}


	Duel::DTextureSamplerObjectPtr DMaterialInstance::getSamplerValue( const DString& paramName )
	{
		SamplerConstantMap::iterator i = mSamplerMap.find(paramName);
		if (i != mSamplerMap.end())
		{
			return i->second;
		}
		return DTextureSamplerObjectPtr();
	}


	void DMaterialInstance::copyTo( DMaterialInstance* inst )
	{
		if (inst != NULL)
		{
			*inst = *this;
		}
	}

	DMaterial::DMaterial( const DString& name) :
		mMtlName(name)
	{
		mPersistInstance = new DMaterialInstance(this);
	}

	DMaterial::~DMaterial()
	{
		delete mPersistInstance;
	}

	const DString& DMaterial::getMaterialName()
	{
		return mMtlName;
	}

	DMaterialInstancePtr DMaterial::createInstance()
	{
		DMaterialInstance* retInstance = new DMaterialInstance(this);
		*retInstance = *mPersistInstance;
		return DMaterialInstancePtr(retInstance);
	}

	void DMaterial::updateGpuParameter( DMaterialInstance* inst, DShaderObject* so )
	{
		if (!so->isValid())
		{
			return;
		}
		// first build the param list, for vs and ps.
		typedef std::vector<DMaterialParameter>	ParamerterList;
		ParamerterList vsParams;
		ParamerterList psParams;
		DMaterialInstance::ParameterIterator pI = inst->getParameterIterator();
		while (pI.hasMoreElements())
		{
			DMaterialParameter& param = pI.getNext();
			if (param.targetGpuProgram == so->getVertexProgram()->getName())
			{
				vsParams.push_back(param);
			}
			else if (param.targetGpuProgram == so->getPixelProgram()->getName())
			{
				psParams.push_back(param);
			}
		}

		// for vertex shader
		DGpuParameters* gpuParam = so->getVertexProgramParameters();
		ParamerterList::iterator i, iend = vsParams.end();
		for (i = vsParams.begin(); i != iend; ++i)
		{
			DMaterialParameter p = *i;
			if (p.isFloat())
			{
				gpuParam->setValue(p.targetGpuParam, inst->getFloatValuePtr(p.physicalIndex), p.elemSize * p.arraySize);
			}
			else if (p.isInt())
			{
				gpuParam->setValue(p.targetGpuParam, inst->getIntValuePtr(p.physicalIndex), p.elemSize * p.arraySize);
			}
			else if (p.isBool())
			{
				gpuParam->setValue(p.targetGpuParam, inst->getBoolValuePtr(p.physicalIndex), p.elemSize * p.arraySize);
			}
			else if (p.isTexture())
			{
				DMaterialInstance::TextureConstantCache c = inst->getTextureValue(p.paramName);
				gpuParam->setValue(p.targetGpuParam, c.second);
			}
			else if (p.isSampler())
			{
				DTextureSamplerObjectPtr s = inst->getSamplerValue(p.paramName);
				gpuParam->setValue(p.targetGpuParam, s);
			}
		}

		// for pixel shader
		gpuParam = so->getPixelProgramParameters();
		iend = psParams.end();
		for (i = psParams.begin(); i != iend; ++i)
		{
			DMaterialParameter p = *i;
			if (p.isFloat())
			{
				gpuParam->setValue(p.targetGpuParam, inst->getFloatValuePtr(p.physicalIndex), p.elemSize * p.arraySize);
			}
			else if (p.isInt())
			{
				gpuParam->setValue(p.targetGpuParam, inst->getIntValuePtr(p.physicalIndex), p.elemSize * p.arraySize);
			}
			else if (p.isBool())
			{
				gpuParam->setValue(p.targetGpuParam, inst->getBoolValuePtr(p.physicalIndex), p.elemSize * p.arraySize);
			}
			else if (p.isTexture())
			{
				DMaterialInstance::TextureConstantCache c = inst->getTextureValue(p.paramName);
				gpuParam->setValue(p.targetGpuParam, c.second);
			}
			else if (p.isSampler())
			{
				DTextureSamplerObjectPtr s = inst->getSamplerValue(p.paramName);
				gpuParam->setValue(p.targetGpuParam, s);
			}
		}
	}

}
