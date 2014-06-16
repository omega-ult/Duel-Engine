//  [1/2/2013 OMEGA] created

#ifndef _DUELPOSTEFFECT_H_
#define _DUELPOSTEFFECT_H_

#include "DuelCommon.h"
#include "DuelRenderable.h"

namespace Duel
{
	enum PostEffectParameterType
	{
		PEPT_Color,
		PEPT_Texture,
		PEPT_Bool,
		PEPT_Int1,
		PEPT_Int2,
		PEPT_Int3,
		PEPT_Int4,
		PEPT_Float1,
		PEPT_Float2,
		PEPT_Float3,
		PEPT_Float4
	};

	struct DPostEffectParameter
	{
		PostEffectParameterType	paramType;

		DString					paramName;
		// element size is the element count of specified parameter type, not the byte size.
		// for example, float4 has 4 elements, 
		uint32					elemSize;

		// physical index in the material, this value will be modified after added into material instance.
		uint32					physicalIndex;

		// what gpu param this param bound to
		DString					targetGpuParam;
		// what shader this parameter bound to.
		DString					targetGpuProgram;

		// whether this parameter is open to user.
		bool					bUserAccess;

		bool					isBool();
		static bool				isBool(PostEffectParameterType type);
		bool					isInt();
		static bool				isInt(PostEffectParameterType type);
		// 		bool					isBool();
		// 		static bool				isBool(MaterialParameterType type);
		bool					isFloat();
		static bool				isFloat(PostEffectParameterType type);
		bool					isColor();
		static bool				isColor(PostEffectParameterType type);
		bool					isTexture();
		static bool				isTexture(PostEffectParameterType type);
		static uint32			getElementSize(PostEffectParameterType type);
		const DString&			getParameterTypeName();
		// query the string for specified type.
		static const DString&	getParameterTypeName(PostEffectParameterType type);
	protected:
		static std::map<PostEffectParameterType, DString>	msParamNameMap;
	};


	class DUEL_API DPostEffectInstance : public DObject
	{
		DUEL_DECLARE_RTTI(DPostEffectInstance)
	public:
		DPostEffectInstance(DPostEffect* parent);

		DPostEffect*	getParent() { return mParent; }

		typedef std::map<DString, DPostEffectParameter>	ParameterMap;
		typedef MapIterator<ParameterMap>				ParameterIterator;
		ParameterIterator	getParameterIterator() { return ParameterIterator(mParamMap); }

		// for now, texture parameter only accept 1 element, meaning the arraySize will be automatically set to 1.
		// for non-texture parameter, the arraySize must be non-zero, otherwise it will be ignored.
		void	addParameter(const DString& name, PostEffectParameterType type, const DString& gpuParam, const DString& gpuProgram,
			bool openToUser = false);
		void	removeParameter(const DString& name);
		void	removeAllParameters();

		// set a bool constant to the program,
		void	setValue(const DString& name, bool val);
		// set a floating constant to the material,
		void	setValue(const DString& name, float val);
		// set a float array, for PEPT_Float[n] type, count should not bigger than 4;
		void	setValue(const DString& name, float* val, uint32 count);
		// set an int constant to the material,
		// this function also works for bool value.
		void	setValue(const DString& name, int val);
		// set an int array, for PEPT_Int[n] type, count should not bigger than 4;
		void	setValue(const DString& name, int* val, uint32 count);
		// sets a DVector2 costant to the material.
		void	setValue(const DString& name, const DVector2& vec);
		// sets a DVector3 costant to the material.
		void	setValue(const DString& name, const DVector3& vec);
		// sets a DVector4 costant to the material.
		void	setValue(const DString& name, const DVector4& vec);
		// set a color value constant to the material.
		void	setValue(const DString& name, const DColor& val);

		// TextureConstant, use <groupName, resourcName> as pair.
		typedef std::pair<DString, DString>	TextureConstant;
		typedef std::pair<TextureConstant, DGpuTextureConstantPtr>	TextureConstantCache;
		// aserious of function overloading for setting values.
		void	setValue(const DString& paramName, TextureConstant tex);

		float*	getFloatValuePtr(uint32 physicalIndex);
		int32*	getIntValuePtr(uint32 physicalIndex);
		int32*	getBoolValuePtr(uint32 physicalIndex);
		TextureConstantCache	getTextureValue(const DString& paramName);
		
		// copy the parameter definitions and the values to another instance.
		// also the postEffect parent will be reset to the current one.
		void	copyTo(DPostEffectInstance* inst);

	protected:
		DPostEffect*	mParent;
		ParameterMap		mParamMap;
		typedef	std::vector<float>	FloatConstantList;
		typedef	std::vector<int32>	IntConstantList;
		typedef std::vector<int32>	BoolConstantList;
		typedef std::map<DString, TextureConstantCache>	TextureConstantMap;
		FloatConstantList	mFloatConstants;
		IntConstantList		mIntConstants;
		BoolConstantList	mBoolConstants;
		TextureConstantMap	mTextureMap;
	};

	class DUEL_API DPostEffect : public DRenderable
	{
		DUEL_DECLARE_RTTI(DPostEffect);
	public:
		DPostEffect(const DString& name);
		virtual ~DPostEffect(); 
		const DString&		getPostEffectName();

		// create an instance, which will be able to used in object.
		virtual DPostEffectInstancePtr	createInstance();

		void	updateGpuParameter(DPostEffectInstance* inst, DShaderObject* so);

		// virtual DRenderTechnique* getRenderTechnique() // leave it to sub class.

	protected:
		DString				mName;
		DPostEffectInstance*	mPersistInstance;
	};
}

#endif