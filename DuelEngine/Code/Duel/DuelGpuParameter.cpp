//  [10/3/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelGpuParameter.h"
#include "DuelAutoGpuParameter.h"
#include "DuelRenderState.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DGpuTextureConstant, DObject);
	DUEL_IMPLEMENT_RTTI_1(DGpuParameters, DObject);

	DGpuParameters::AutoConstantMap buildAutoConstDict() 
	{
		DGpuParameters::AutoConstantMap table;
		table.insert(
			std::make_pair("Auto_WorldMatrix", std::make_pair(GCT_Matrix4x4, AGPN_WorldMatrix)));
		table.insert(
			std::make_pair("Auto_InvWorldMatrix", std::make_pair(GCT_Matrix4x4, AGPN_InvWorldMatrix)));

		table.insert(
			std::make_pair("Auto_ViewMatrix", std::make_pair(GCT_Matrix4x4, AGPN_ViewMatrix)));
		table.insert(
			std::make_pair("Auto_InvViewMatrix", std::make_pair(GCT_Matrix4x4, AGPN_InvViewMatrix)));

		table.insert(
			std::make_pair("Auto_ProjMatrix", std::make_pair(GCT_Matrix4x4, AGPN_ProjMatrix)));
		table.insert(
			std::make_pair("Auto_InvProjMatrix", std::make_pair(GCT_Matrix4x4, AGPN_InvProjMatrix)));

		table.insert(
			std::make_pair("Auto_ViewProjMatrix", std::make_pair(GCT_Matrix4x4, AGPN_ViewProjMatrix)));
		table.insert(
			std::make_pair("Auto_InvViewProjMatrix", std::make_pair(GCT_Matrix4x4, AGPN_InvViewProjMatrix)));

		table.insert(
			std::make_pair("Auto_WorldViewMatrix", std::make_pair(GCT_Matrix4x4, AGPN_WorldViewMatrix)));
		table.insert(
			std::make_pair("Auto_InvWorldViewMatrix", std::make_pair(GCT_Matrix4x4, AGPN_InvWorldViewMatrix)));

		table.insert(
			std::make_pair("Auto_WorldViewProjMatrix", std::make_pair(GCT_Matrix4x4, AGPN_WorldViewProjMatrix)));
		table.insert(
			std::make_pair("Auto_InvWorldViewProjMatrix", std::make_pair(GCT_Matrix4x4, AGPN_InvWorldViewProjMatrix)));


		return table;
	}

	const DGpuParameters::AutoConstantMap DGpuParameters::AutoConstantDictionary = buildAutoConstDict();
	//--------------------------------------------------------
	DGpuParameters::DGpuParameters( const DString& name ) :
		mName(name),
		mAutoParamMask(0)
	{

	}

	void DGpuParameters::setName( const DString& name )
	{
		mName = name;
	}

	void DGpuParameters::addConstant( const DString& name, GpuConstantDefinition& constDef )
	{
		if (constDef.isTexture())
		{
			mTexConstants.insert(TextureConstantMap::value_type(name, DGpuTextureConstantPtr()));
		}
		if (constDef.isSampler())
		{
			mSampConstants.insert(SamplerConstantMap::value_type(name, DTextureSamplerObjectPtr()));
		}


		if (constDef.isInt())
		{
			constDef.physicalIndex = mIntConstants.size();
			mIntConstants.insert(mIntConstants.end(), constDef.arraySize*constDef.elementSize, 0);
		}
		else if (constDef.isFloat())
		{
			constDef.physicalIndex = mFloatConstants.size();
			mFloatConstants.insert(mFloatConstants.end(), constDef.arraySize*constDef.elementSize, 0.0f);
		}
		else if (constDef.isBool())
		{
			constDef.physicalIndex = mBoolConstants.size();
			mBoolConstants.insert(mBoolConstants.end(), constDef.arraySize*constDef.elementSize, 0);
		}
		
		AutoConstantMap::const_iterator i = AutoConstantDictionary.find(name);
		if (i != AutoConstantDictionary.end())
		{
			mAutoConstantsMap[name] = constDef;
			mAutoParamMask |= i->second.second;
		}
		else
		{
			mConstantsMap[name] = constDef;
		}
	}

	void DGpuParameters::removeConstant( const DString& name )
	{
		// remove if we find.
		AutoConstantMap::const_iterator ci = AutoConstantDictionary.find(name);
		GpuConstantMap::iterator i = mAutoConstantsMap.find(name);
		if (i != mAutoConstantsMap.end())
		{
			mAutoConstantsMap.erase(i);
			mAutoParamMask &= ~(ci->second.second);
			return;
		}
		i = mConstantsMap.find(name);
		if (i != mConstantsMap.end())
		{
			mConstantsMap.erase(i);
			return;
		}
		TextureConstantMap::iterator ti = mTexConstants.find(name);
		if (ti != mTexConstants.end())
		{
			mTexConstants.erase(ti);
			return;
		}
		SamplerConstantMap::iterator si = mSampConstants.find(name);
		if (si != mSampConstants.end())
		{
			mSampConstants.erase(si);
		}
	}

	void DGpuParameters::removeAllConstants( void )
	{
		mConstantsMap.clear();
		mAutoConstantsMap.clear();
		mTexConstants.clear();
	}

	const GpuConstantDefinition DGpuParameters::getConstant( const DString& name )
	{
		GpuConstantDefinition ret;
		GpuConstantMap::iterator i = mAutoConstantsMap.find(name);
		if (i == mAutoConstantsMap.end())
		{
			i = mConstantsMap.find(name);
			if (i == mConstantsMap.end())
			{
				return ret;
			}
		}
		return ret;
	}

	void DGpuParameters::setValue( const DString& name, float val )
	{
		GpuConstantMap::iterator i = mAutoConstantsMap.find(name);
		// find auto constants first.
		if (i == mAutoConstantsMap.end())
		{
			i = mConstantsMap.find(name);
			if (i == mConstantsMap.end())
			{
				return;
			}
		}

		if (i->second.isFloat())
		{
			mFloatConstants[i->second.physicalIndex] = val;
		}
		else if (i->second.isInt())
		{
			mIntConstants[i->second.physicalIndex] = static_cast<int>(val);
		}
	}

	void DGpuParameters::setValue( const DString& name, int val )
	{
		GpuConstantMap::iterator i = mAutoConstantsMap.find(name);
		// find auto constants first.
		if (i == mAutoConstantsMap.end())
		{
			i = mConstantsMap.find(name);
			if (i == mConstantsMap.end())
			{
				return;
			}
		}
		if (i->second.isFloat())
		{
			mFloatConstants[i->second.physicalIndex] = static_cast<float>(val);
		}
		else if (i->second.isInt())
		{
			mIntConstants[i->second.physicalIndex] = val;
		}
		else if (i->second.isBool())
		{
			mBoolConstants[i->second.physicalIndex] = val;
		}
	}

	void DGpuParameters::setValue( const DString& name, bool val )
	{
		GpuConstantMap::iterator i = mAutoConstantsMap.find(name);
		// find auto constants first.
		if (i == mAutoConstantsMap.end())
		{
			i = mConstantsMap.find(name);
			if (i == mConstantsMap.end())
			{
				return;
			}
		}
		if (i->second.isBool())
		{
			mBoolConstants[i->second.physicalIndex] = val ? 1 : 0;
		}
	}

	void DGpuParameters::setValue( const DString& name, const DVector4& vec )
	{
		GpuConstantMap::iterator i = mAutoConstantsMap.find(name);
		// find auto constants first.
		if (i == mAutoConstantsMap.end())
		{
			i = mConstantsMap.find(name);
			if (i == mConstantsMap.end())
			{
				return;
			}
		}
		uint32 baseIndex = i->second.physicalIndex;
		if (i->second.isFloat())
		{
			for(uint32 i = 0; i < 4; ++i)
			{
				mFloatConstants[baseIndex+i] = vec[i];
			}
		}
		else if (i->second.isInt())
		{
			for(uint32 i = 0; i < 4; ++i)
			{
				mIntConstants[baseIndex+i] = static_cast<int>(vec[i]);
			}
		}
	
	}

	void DGpuParameters::setValue( const DString& name, const DMatrix4& mat )
	{
		GpuConstantMap::iterator i = mAutoConstantsMap.find(name);
		// find auto constants first.
		if (i == mAutoConstantsMap.end())
		{
			i = mConstantsMap.find(name);
			if (i == mConstantsMap.end())
			{
				return;
			}
		}
		
		if (i->second.isFloat())
		{
			assert(i->second.physicalIndex + i->second.elementSize <= mFloatConstants.size());
			memcpy(&mFloatConstants[i->second.physicalIndex], mat[0], i->second.elementSize * sizeof(float));
		}
	}

	void DGpuParameters::setValue( const DString& name, float* val, size_t count )
	{
		GpuConstantMap::iterator i = mAutoConstantsMap.find(name);
		// find auto constants first.
		if (i == mAutoConstantsMap.end())
		{
			i = mConstantsMap.find(name);
			if (i == mConstantsMap.end())
			{
				return;
			}
		}
		if (i->second.isFloat())
		{
			memcpy(&mFloatConstants[i->second.physicalIndex], val, count * sizeof(float));
		}
	}

	void DGpuParameters::setValue( const DString& name, int* val, size_t count )
	{
		GpuConstantMap::iterator i = mAutoConstantsMap.find(name);
		// find auto constants first.
		if (i == mAutoConstantsMap.end())
		{
			i = mConstantsMap.find(name);
			if (i == mConstantsMap.end())
			{
				return;
			}
		}
		if (i->second.isInt())
		{
			assert(i->second.physicalIndex + i->second.elementSize * count <= mIntConstants.size());
			memcpy(&mIntConstants[i->second.physicalIndex], val, count * sizeof(int32));
		}
		else if (i->second.isBool())
		{
			assert(i->second.physicalIndex + i->second.elementSize * count <= mBoolConstants.size());
			memcpy(&mBoolConstants[i->second.physicalIndex], val, count * sizeof(int32));
		}
	}

	void DGpuParameters::setValue( const DString& name, DMatrix4* mat, size_t count )
	{
		GpuConstantMap::iterator i = mAutoConstantsMap.find(name);
		// find auto constants first.
		if (i == mAutoConstantsMap.end())
		{
			i = mConstantsMap.find(name);
			if (i == mConstantsMap.end())
			{
				return;
			}
		}

		if (i->second.isFloat())
		{
			assert(i->second.physicalIndex + i->second.elementSize * count <= mFloatConstants.size());
			for (uint32 c = 0; c < count; ++c)
			{
				memcpy(&mFloatConstants[i->second.physicalIndex + i->second.elementSize * c * sizeof(float)], mat[c][0], i->second.elementSize * sizeof(float));
			}
			
		}
	}

	void DGpuParameters::setValue( const DString& name, const DColor& val )
	{
		GpuConstantMap::iterator i = mAutoConstantsMap.find(name);
		// find auto constants first.
		if (i == mAutoConstantsMap.end())
		{
			i = mConstantsMap.find(name);
			if (i == mConstantsMap.end())
			{
				return;
			}
		}
		if (i->second.isFloat())
		{
			memcpy(&mFloatConstants[i->second.physicalIndex], val.ptr(), 4 * sizeof(float));
		}
	}

	void DGpuParameters::setValue( const DString& name, DGpuTextureConstantPtr val )
	{
		TextureConstantMap::iterator i = mTexConstants.find(name);
		if (i != mTexConstants.end())
		{
			i->second = val;
		}
	}

	void DGpuParameters::setValue( const DString& name, DTextureSamplerObjectPtr val )
	{
		SamplerConstantMap::iterator i = mSampConstants.find(name);
		if (i != mSampConstants.end())
		{
			i->second = val;
		}
	}

	void DGpuParameters::setValue( const DString& name, const DVector2& m1_, const DVector2& m2_ )
	{
		float tmp[4];
		tmp[0] = m1_.x; tmp[1] = m1_.y;
		tmp[2] = m2_.x; tmp[3] = m2_.y;
		setValue(name, tmp, 4);
	}

	void DGpuParameters::setValue( const DString& name, const DVector2& m1_, const DVector2& m2_, const DVector2& m3_ )
	{
		float tmp[6];
		tmp[0] = m1_.x; tmp[1] = m1_.y;
		tmp[2] = m2_.x; tmp[3] = m2_.y;
		tmp[4] = m3_.x; tmp[5] = m3_.y;
		setValue(name, tmp, 6);
	}

	void DGpuParameters::setValue( const DString& name, const DVector2& m1_, const DVector2& m2_, const DVector2& m3_, const DVector2& m4_ )
	{
		float tmp[8];
		tmp[0] = m1_.x; tmp[1] = m1_.y;
		tmp[2] = m2_.x; tmp[3] = m2_.y;
		tmp[4] = m3_.x; tmp[5] = m3_.y;
		tmp[6] = m3_.x; tmp[7] = m3_.y;
		setValue(name, tmp, 8);
	}

	void DGpuParameters::setValue( const DString& name, const DVector3& m1_, const DVector3& m2_ )
	{
		float tmp[6];
		tmp[0] = m1_.x; tmp[1] = m1_.y; tmp[2] = m1_.z;
		tmp[3] = m2_.x; tmp[4] = m2_.y; tmp[5] = m2_.z;
		setValue(name, tmp, 6);
	}

	void DGpuParameters::setValue( const DString& name, const DMatrix3& mat )
	{
		float tmp[9];
		tmp[0] = mat[0][0]; tmp[1] = mat[0][1]; tmp[2] = mat[0][2]; 
		tmp[3] = mat[1][0]; tmp[4] = mat[1][1]; tmp[5] = mat[1][2]; 
		tmp[6] = mat[2][0]; tmp[7] = mat[2][1]; tmp[8] = mat[2][2]; 
		setValue(name, tmp, 9);
	}

	void DGpuParameters::setValue( const DString& name, const DVector3& m1_, const DVector3& m2_, const DVector3& m3_, const DVector3& m4_ )
	{
		float tmp[12];
		tmp[0] = m1_.x; tmp[1] = m1_.y; tmp[2] = m1_.z;
		tmp[3] = m2_.x; tmp[4] = m2_.y; tmp[5] = m2_.z;
		tmp[6] = m3_.x; tmp[7] = m3_.y; tmp[8] = m3_.z;
		tmp[9] = m4_.x; tmp[10] = m4_.y; tmp[11] = m4_.z;
		setValue(name, tmp, 12);
	}

	void DGpuParameters::setValue( const DString& name, const DVector4& m1_, const DVector4& m2_ )
	{
		float tmp[8];
		tmp[0] = m1_.x; tmp[1] = m1_.y; tmp[2] = m1_.z; tmp[3] = m1_.w;
		tmp[4] = m2_.x; tmp[5] = m2_.y; tmp[6] = m2_.z; tmp[7] = m2_.w;
		setValue(name, tmp, 8);
	}

	void DGpuParameters::setValue( const DString& name, const DVector4& m1_, const DVector4& m2_, const DVector4& m3_ )
	{
		float tmp[12];
		tmp[0] = m1_.x; tmp[1] = m1_.y; tmp[2] = m1_.z; tmp[3] = m1_.w;
		tmp[4] = m2_.x; tmp[5] = m2_.y; tmp[6] = m2_.z; tmp[7] = m2_.w;
		tmp[8] = m3_.x; tmp[9] = m3_.y; tmp[10] = m3_.z; tmp[11] = m3_.w;
		setValue(name, tmp, 12);
	}

	size_t DGpuParameters::getBufferSize()
	{
		return mFloatConstants.size() + mIntConstants.size();
	}

	float* DGpuParameters::getFloatValuePtr( uint32 physicalIndex )
	{
		return &mFloatConstants[physicalIndex];
	}

	int* DGpuParameters::getIntValuePtr( uint32 physicalIndex )
	{
		return &mIntConstants[physicalIndex];
	}

	int32* DGpuParameters::getBoolValuePtr( uint32 physicalIndex )
	{
		return &mBoolConstants[physicalIndex];
	}


	uint64 DGpuParameters::getAutoParameterMask()
	{
		return mAutoParamMask;
	}

	void DGpuParameters::setAutoParameterMask( uint64 mask )
	{
		mAutoParamMask = mask;
	}

	DGpuTextureConstantPtr DGpuParameters::getTextureConstant( const DString& name )
	{
		TextureConstantMap::iterator i = mTexConstants.find(name);
		if (i == mTexConstants.end())
		{
			return DGpuTextureConstantPtr();
		}
		return i->second;
	}

	DTextureSamplerObjectPtr DGpuParameters::getSamplerConstant( const DString& name )
	{
		SamplerConstantMap::iterator i = mSampConstants.find(name);
		if (i == mSampConstants.end())
		{
			return DTextureSamplerObjectPtr();
		}
		return i->second;
	}

	size_t GpuConstantDefinition::getElementByteSize( GpuConstantType ctype, bool padToMultiplesOf4 )
	{
		if (padToMultiplesOf4)
		{
			switch(ctype)
			{
			case GCT_Bool:
			case GCT_Float1:
			case GCT_Int1:
			case GCT_Texture:
			case GCT_Float2:
			case GCT_Int2:
			case GCT_Float3:
			case GCT_Int3:
			case GCT_Float4:
			case GCT_Int4:
				return 16;
			case GCT_Matrix2x2:
			case GCT_Matrix2x3:
			case GCT_Matrix2x4:
				return 32; // 2 float4s
			case GCT_Matrix3x2:
			case GCT_Matrix3x3:
			case GCT_Matrix3x4:
				return 48; // 3 float4s
			case GCT_Matrix4x2:
			case GCT_Matrix4x3:
			case GCT_Matrix4x4:
				return 64; // 4 float4s
			default:
				return 16;
			};
		}
		else
		{
			switch(ctype)
			{
			case GCT_Bool:
			case GCT_Float1:
			case GCT_Int1:
			case GCT_Texture:
				return 4;
			case GCT_Float2:
			case GCT_Int2:
				return 8;
			case GCT_Float3:
			case GCT_Int3:
				return 12;
			case GCT_Float4:
			case GCT_Int4:
				return 16;
			case GCT_Matrix2x2:
				return 16;
			case GCT_Matrix2x3:
			case GCT_Matrix3x2:
				return 24;
			case GCT_Matrix2x4:
			case GCT_Matrix4x2:
				return 32; 
			case GCT_Matrix3x3:
				return 36;
			case GCT_Matrix3x4:
			case GCT_Matrix4x3:
				return 48; 
			case GCT_Matrix4x4:
				return 64; 
			default:
				return 4;
			};

		}
	}

	size_t GpuConstantDefinition::getElementSize( GpuConstantType ctype, bool padToMultiplesOf4 )
	{
		return getElementByteSize(ctype,padToMultiplesOf4)/4;
	}

	bool GpuConstantDefinition::isSampler( GpuConstantType c )
	{
		if (c == GCT_Sampler)
		{
			return true;
		}
		return false;
	}

	bool GpuConstantDefinition::isSampler() const
	{
		return isSampler(constType);
	}

	bool GpuConstantDefinition::isTexture( GpuConstantType c )
	{
		return c == GCT_Texture;
	}

	bool GpuConstantDefinition::isTexture() const
	{
		return isTexture(constType);
	}

	bool GpuConstantDefinition::isInt( GpuConstantType c )
	{
		switch(c)
		{
		case GCT_Int1:
		case GCT_Int2:
		case GCT_Int3:
		case GCT_Int4:
			return true;
		default:
			return false;
		};
	}

	bool GpuConstantDefinition::isInt() const
	{
		return isInt(constType);
	}

	bool GpuConstantDefinition::isMatrix( GpuConstantType c )
	{
		switch (c)
		{
		case GCT_Matrix2x2:
		case GCT_Matrix2x3:
		case GCT_Matrix2x4:
		case GCT_Matrix3x2:
		case GCT_Matrix3x3:
		case GCT_Matrix3x4:
		case GCT_Matrix4x2:
		case GCT_Matrix4x3:
		case GCT_Matrix4x4:
			return true;
		default:
			return false;
		}
		return false;
	}

	bool GpuConstantDefinition::isBool( GpuConstantType c )
	{
		return c == GCT_Bool;
	}

	bool GpuConstantDefinition::isBool()
	{
		return isBool(constType);
	}

	bool GpuConstantDefinition::isMatrix() const
	{
		return isMatrix(constType);
	}

	bool GpuConstantDefinition::isFloat( GpuConstantType c )
	{
		switch(c)
		{
		case GCT_Bool:
		case GCT_Int1:
		case GCT_Int2:
		case GCT_Int3:
		case GCT_Int4:
		case GCT_Texture:
		case GCT_Sampler:
			return false;
		default:
			return true;
		};
	}

	bool GpuConstantDefinition::isFloat() const
	{
		return isFloat(constType);
	}

	GpuConstantDefinition::GpuConstantDefinition() : constType(GCT_Unknown)
		, physicalIndex(0)
		, elementSize(0)
		, arraySize(1)
		, offset(0)
		, slot(0)
	{

	}

}
