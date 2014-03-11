//  [9/2/2012 OMEGA] created

#ifndef _DUELGPUPARAMETER_H_
#define _DUELGPUPARAMETER_H_

#include "DuelCommon.h"
#include "DuelColor.h"
#include "DuelRenderState.h"

namespace Duel
{

	enum GpuConstantType
	{
		GCT_Float1 = 1,
		GCT_Float2 = 2,
		GCT_Float3 = 3,
		GCT_Float4 = 4,
		GCT_Texture1D = 5,
		GCT_Texture2D = 6,
		GCT_Texture3D = 7,
		GCT_TextureCube = 8,
		GCT_Sampler	= 9,
		GCT_Matrix2x2 = 11,
		GCT_Matrix2x3 = 12,
		GCT_Matrix2x4 = 13,
		GCT_Matrix3x2 = 14,
		GCT_Matrix3x3 = 15,
		GCT_Matrix3x4 = 16,
		GCT_Matrix4x2 = 17,
		GCT_Matrix4x3 = 18,
		GCT_Matrix4x4 = 19,
		GCT_Int1 = 20,
		GCT_Int2 = 21,
		GCT_Int3 = 22,
		GCT_Int4 = 23,
		GCT_Unknown = 99
	};


	/** Information about predefined program constants. 
	@note Only available for high-level programs but is referenced generically
	by GpuProgramParameters.
	*/
	struct DUEL_API GpuConstantDefinition
	{

		GpuConstantDefinition();
		/// Data type
		GpuConstantType constType;
		/// Physical start index in system memory buffer (either float or int buffer), usually
		// in the GpuParameters::mXXXConstants XXX: Float or Int
		size_t physicalIndex;
		
		/// Logical index - used to communicate this constant to the rendersystem
		// for DX10 and higher, this value indicate the constant index in the constant buffer.
		size_t logicalIndex;
		
		/** Number of raw buffer slots per element 
		(some programs pack each array element to float4, some do not) */
		size_t elementSize;
		/// Length of array
		size_t arraySize;


		// extension for DX10 and higher, constant offset used to indicate
		// the constant' start position in the constant buffer, for dx9, just set it 0.
		uint32	offset;
		// extension for DX10 and higher, constant slot used to indicate
		// the constant buffer slot, for dx9 or openGL, just set it 0.
		uint32	slot;

		//---------------------
		bool		isFloat() const;

		static bool	isFloat(GpuConstantType c);
		bool		isMatrix() const;
		static bool	isMatrix(GpuConstantType c);
		bool		isInt() const;

		static bool	isInt(GpuConstantType c);

		bool		isTexture() const;

		static bool	isTexture(GpuConstantType c);
		bool		isSampler() const;
		static bool	isSampler(GpuConstantType c);
		static size_t	getElementSize(GpuConstantType ctype, bool padToMultiplesOf4);
		/** Get the element size of a given type, including whether to pad the 
			elements into multiples of 4 (e.g. SM1 and D3D does, GLSL doesn't)
		*/
		static size_t getElementByteSize(GpuConstantType ctype, bool padToMultiplesOf4);

	};

	// as we know RenderTargets and Textures are all available set as gpu texture constant,
	// so they must override this function to provide hardware-related value.
	class DUEL_API DGpuTextureConstant : public DObject
	{
		DUEL_DECLARE_RTTI(DGpuTextureConstant)
	public:
		virtual void			setSamplerState(const DTextureSamplerState& state) = 0;
	};


	// do not support low level shader language.
	class DUEL_API DGpuParameters : public DObject
	{
	DUEL_DECLARE_RTTI(DGpuParameters)
	public:
		DGpuParameters(const DString& name);
		virtual ~DGpuParameters() {};

		// the public dictionary of auto constants, those string is reserved in 
		// shader language, when you declare a variable with name included in this
		// dictionary, it will be regarded as a auto constant, and it will be updated
		// automatically.
		typedef	std::map<DString, std::pair<GpuConstantType,uint64>>	AutoConstantMap;
		static const 	AutoConstantMap	AutoConstantDictionary;

		// set the name of this parameter.
		void			setName(const DString& name);
		const DString&	getName() { return mName; }
		
		// add a new constant, with a unique name. the physicalIndex of the constant will
		// be auto generated, but other properties should be filled before adding.
		virtual void	addConstant(const DString& name, GpuConstantDefinition& constDef);
		// remove a constant using its name.
		virtual void	removeConstant(const DString& name);
		// remove all constants
		virtual void	removeAllConstants(void);

		// get the constant definition of given name.
		const GpuConstantDefinition	getConstant(const DString& name);

		// these two functions are designed for traversing all constants 
		typedef HashMap<DString, GpuConstantDefinition> GpuConstantMap;
		typedef	MapIterator<GpuConstantMap>	GpuConstantIterator;
		GpuConstantIterator	getAutoGpuConstantIterator() { return GpuConstantIterator(mAutoConstantsMap.begin(), mAutoConstantsMap.end()); }
		GpuConstantIterator	getCustomGpuConstantIterator() { return GpuConstantIterator(mConstantsMap.begin(), mConstantsMap. end()); }
		

		// set a floating constant to the program,
		virtual void	setValue(const DString& name, float val);
		// set an int constant to the program,
		virtual void	setValue(const DString& name, int val);
		// sets a DVector4 costant to the program.
		virtual void	setValue(const DString& name, const DVector4& vec);
		// sets a DMatrix4 costant to the program.
		virtual void	setValue(const DString& name, const DMatrix4& mat);

		//  [11/11/2013 OMEGA] 新增不同的参数设置方式.
		// set a GCT_Matrix2x2 value.
		virtual void	setValue(const DString& name, const DVector2& m1_, const DVector2& m2_);
		// set a GCT_Matrix2x3 value.
		virtual void	setValue(const DString& name, const DVector2& m1_, const DVector2& m2_, const DVector2& m3_);
		// set a GCT_Matrix2x4 value.
		virtual void	setValue(const DString& name, const DVector2& m1_, const DVector2& m2_, const DVector2& m3_, const DVector2& m4_);

		// set a GCT_Matrix3x2 value.
		virtual void	setValue(const DString& name, const DVector3& m1_, const DVector3& m2_);
		// set a GCT_Matrix3x3 value.
		virtual void	setValue(const DString& name, const DMatrix3& mat);
		// set a GCT_Matrix3x4 value.
		virtual void	setValue(const DString& name, const DVector3& m1_, const DVector3& m2_, const DVector3& m3_, const DVector3& m4_);

		// set a GCT_Matrix4x2 value.
		virtual void	setValue(const DString& name, const DVector4& m1_, const DVector4& m2_);
		// set a GCT_Matrix4x3 value.
		virtual void	setValue(const DString& name, const DVector4& m1_, const DVector4& m2_, const DVector4& m3_);



		// set a floating array constant to the program,
		virtual void	setValue(const DString& name, float* val, size_t count);		
		// set a int array constant to the program,
		virtual void	setValue(const DString& name, int* val, size_t count);
		// set a DMatrix4 array costant to the program.
		virtual void	setValue(const DString& name, DMatrix4* mat, size_t count);
		// set a color value constant to the program.
		virtual void	setValue(const DString& name, const DColor& val);
		// set a texture value to the program.
		virtual	void	setValue(const DString& name, DGpuTextureConstant*	val);
		// set a sampler value.
		virtual	void	setValue(const DString& name, const DTextureSamplerState& val);

		// get the buffer size the constants takes
		size_t			getBufferSize();

		// get the float value pointer using specified index, used in binding constants in graphic card.
		virtual float*	getFloatValuePtr(uint32 physicalIndex);
		// get the int value pointer using specified index, used in binding constants in graphic card.
		virtual int*	getIntValuePtr(uint32 physicalIndex);
		typedef	std::map<DString, DGpuTextureConstant*>	TextureConstantMap;
		typedef	MapIterator<TextureConstantMap>	TextureConstantIterator;
		TextureConstantIterator	getTextureConstantIterator() { return TextureConstantIterator(mTexConstants.begin(), mTexConstants.end()); }
		DGpuTextureConstant*	getTextureConstant(const DString& name);
		typedef	std::map<DString, DTextureSamplerState>	SamplerConstantMap;
		typedef	MapIterator<SamplerConstantMap>	SamplerConstantIterator;
		SamplerConstantIterator	getSamplerConstantIterator() { return SamplerConstantIterator(mSampConstants.begin(), mSampConstants.end()); }
		DTextureSamplerState	getSamplerConstant(const DString& name);
		// bit set which indicate whether the parameter contains auto-parameter
		uint64			getAutoParameterMask();
		// just leave this interface for convenience, the mask is constructed automatically,
		// DO NOT call it yourself.
		void			setAutoParameterMask(uint64 mask);

	protected:	
		// these constants must be set manually.
		GpuConstantMap	mConstantsMap;
		// auto constants list, these value will be auto updated.
		GpuConstantMap	mAutoConstantsMap;
		// the name should be set the same as the gpu program.
		DString			mName;
		uint64			mAutoParamMask;
		// constant list used to store temp value, when the program
		// use setValue(), the value will be written here instead of
		// video card, the gpu constant binding procedure should be 
		// done in rendersystem, where the value is transfered to the
		// video card.
		typedef	std::vector<float>	FloatConstantList;
		typedef	std::vector<int32>	IntConstantList;
		FloatConstantList	mFloatConstants;
		IntConstantList		mIntConstants;

		// store textures.
		TextureConstantMap	mTexConstants;
		// store samplers.
		SamplerConstantMap	mSampConstants;
	};


}


#endif