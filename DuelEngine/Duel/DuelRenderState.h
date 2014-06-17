//  [8/9/2012 OMEGA] created

#ifndef	_DUELRENDERSTATE_H_
#define _DUELRENDERSTATE_H_

#include "DuelCommon.h"
#include "DuelColor.h"
#include "DuelTexture.h"

namespace Duel
{
	// TODO: 这里以后要为渲染系统提供从State到底层State的Map支持, 也即map<State, D3DState*>的支持,
	// 这里需要提供一个Map的比较函数, 提供一个operator<即可.

	
	/** Filtering options for textures / mipmaps. */
	enum FilterOption
	{
		/// No filtering, used for FILT_MIP to turn off mipmapping
		FO_None,
		/// Use the closest pixel
		FO_Point,
		/// Average of a 2x2 pixel area, denotes bilinear for MIN and MAG, trilinear for MIP
		FO_Linear,
		/// Similar to FO_LINEAR, but compensates for the angle of the texture plane
		FO_Anistropic
	};

	/** Light shading modes. */
	enum ShadeMode
	{
		SM_Flat,
		SM_Gouraud,
		SM_Phong
	};


	/** Hardware culling modes based on vertex winding.
        This setting applies to how the hardware API culls triangles it is sent. */
    enum CullingMode
    {
        /// Hardware never culls triangles and renders everything it receives.
        CM_None = 1,
        /// Hardware culls triangles whose vertices are listed clockwise in the view (default).
        CM_ClockWise = 2,
        /// Hardware culls triangles whose vertices are listed anticlockwise in the view.
        CM_AntiClockWise
    };

	/** The polygon mode to use when rasterising. */
	enum PolygonMode
	{
		/// Only points are rendered.
		PM_Points = 1,
		/// Wireframe models are rendered.
		PM_Wireframe = 2,
		/// Solid polygons are rendered.
		PM_Solid = 3
	};


	
	/// Enum describing the various actions which can be taken onthe stencil buffer
	enum StencilOperation
	{
		/// Leave the stencil buffer unchanged
		SO_Keep,
		/// Set the stencil value to zero
		SO_Zero,
		/// Set the stencil value to the reference value
		SO_Replace,
		/// Increase the stencil value by 1, clamping at the maximum value
		SO_Increment,
		/// Decrease the stencil value by 1, clamping at 0
		SO_Decrement,
		/// Increase the stencil value by 1, wrapping back to 0 when incrementing the maximum value
		SO_IncrementWrap,
		/// Decrease the stencil value by 1, wrapping when decrementing 0
		SO_DecrementWrap,
		/// Invert the bits of the stencil buffer
		SO_Invert
	};

	/** Comparison functions used for the depth/stencil buffer operations and 
	others. */
    enum CompareFunction
    {
        CF_AlwaysFail,
        CF_AlwaysPass,
        CF_Less,
        CF_LessEqual,
        CF_Equal,
        CF_NotEqual,
        CF_GreaterEqual,
        CF_Greater
    };



	// blend operation used in blending
	enum BlendOperation
	{
		BO_Add,		// Add source 1 and source 2.
		BO_Sub,		// Subtract source 1 from source 2.
		BO_RevSub,	// Subtract source 2 from source 1.
		BO_Min,		// Find the minimum of source 1 and source 2.
		BO_Max		// Find the maximum of source 1 and source 2.
	};
	// blend factor used in blending.
	enum BlendFactor
	{
		BF_Zero,
		BF_One,
		BF_SrcAlpha,
		BF_DstAlpha,
		BF_InvSrcAlpha,
		BF_InvDstAlpha,
		BF_SrcColor,
		BF_DstColor,
		BF_InvSrcColor,
		BF_InvDstColor,
		BF_SrcAlphaSat
	};
	

	struct DUEL_API DRasterizerState
	{
		// default values are set in constructor-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		DRasterizerState();
		DRasterizerState(const DRasterizerState& rs) { operator = (rs); }
		bool			operator == (const DRasterizerState& rs);
		bool			operator != (const DRasterizerState& rs) { return !operator == (rs);}
		// DRasterizerState-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		PolygonMode		polygonMode;			// default:PM_Solid	
		ShadeMode		shadeMode;				// default:SM_Gouraud	
		CullingMode		cullingMode;			// default:CM_Clockwise	
		// solving z-fighting-=-=-=-=-=-=-=-=--=-=-=-=-=-
		float			polygonOffsetFactor;	// default:0.0f
		float			polygonOffsetUnit;		// default:0.0f
		// enable/disable scissor test=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		bool			scissorEnable;			// default: false
		// enable/disable multisample=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		bool			multisampleEnable;		// default: true	
		// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	};

	struct DUEL_API DDepthStencilState
	{
		// default values are set in constructor-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		DDepthStencilState();
		DDepthStencilState(const DDepthStencilState& dss) { operator=(dss); }
		DDepthStencilState&	operator = (const DDepthStencilState& dss);
		bool				operator == (const DDepthStencilState& dss);
		bool				operator != (const DDepthStencilState& dss){ return !operator==(dss); }
		// DDepthStencilState=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		// enable/disable depth test=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		bool				depthEnable;		// default: true	
		// enable/disable depth write=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		bool				depthWriteEnable;	// default: true
		// the depth's comparation function
		CompareFunction		depthComparison;		// default: CF_Less
		//
		// enable/disable front stencil=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		bool				frontStencilEnable;		// default: false
		// front stencil's comparation function=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		CompareFunction		frontStencilComparison;	// default: CF_AlwaysPass
		// front stencil's mask=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		uint32				frontStencilReadMask;		// default: 0xffffffff
		uint32				frontStencilWriteMask;		// default: 0xffffffff
		// front stencil's operations=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		StencilOperation	frontStencilFail;		// default: SO_Keep
		StencilOperation	frontStencilDepthFail;	// default: SO_Keep
		StencilOperation	frontStencilPass;		// default: SO_Keep
		//
		// enable/disable back stencil=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		bool				backStencilEnable;		// default: false
		// back stencil's comparation function=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		CompareFunction		backStencilComparison;	// default: CF_ALWAYS_PASS
		// back stencil's mask=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		uint32				backStencilReadMask;	// default: 0xffffffff
		uint32				backStencilWriteMask;	// default: 0xffffffff
		// back stencil's operations=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		StencilOperation	backStencilFail;		// default: SO_KEEP
		StencilOperation	backStencilDepthFail;	// default: SO_KEEP
		StencilOperation	backStencilPass;		// default: SO_KEEP
	};


	enum ColorWriteMask
	{
		CWM_Red		= 1UL,
		CWM_Green	= 2UL,
		CWM_Blue	= 4UL,
		CWM_Alpha	= 8UL,
		CWM_All		= CWM_Red | CWM_Green | CWM_Blue | CWM_Alpha
	};

	//  [3/29/2013 OMEGA] 新加的混合状态, 按照DX10+的风格写, 单独处理每个RTBlendState
	struct DUEL_API DRenderTargetBlendState
	{
		// default values are set in constructor-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		DRenderTargetBlendState();
		DRenderTargetBlendState(const DRenderTargetBlendState& sbs) { operator=(sbs); }
		bool							operator ==(const DRenderTargetBlendState& sbs);
		bool							operator != (const DRenderTargetBlendState& sbs) { return !operator==(sbs); }
		// blend enable=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		bool				blendEnable;			// default: false
		// color blend default as alpha blending =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		BlendOperation		colorBlendOpeartion;	// default: BO_Add
		BlendFactor			colorSrcFactor;			// default: BF_SrcAlpha
		BlendFactor			colorDstFactor;			// default: BF_InvSrcAlpha
		// alpha blend  default as alpha blending =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		BlendOperation		alphaBlendOpeartion;	// default: B0_Add
		BlendFactor			alphaSrcFactor;			// default: BF_One
		BlendFactor			alphaDstFactor;			// default: BF_InvSrcAlpha
		uint8				colorWriteMask;			// default:	CWM_All

	};
	struct DUEL_API DBlendState
	{
		DBlendState();
		DBlendState(const DBlendState& sbs) { operator=(sbs); }
		bool				operator ==(const DBlendState& sbs);
		bool				operator != (const DBlendState& sbs) { return !operator==(sbs); }
#define DUEL_MAX_BLEND_LAYERS	8
		// enable/disable alpha to coverage=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		bool				alphaToCoverageEnable;	// default: false

		DRenderTargetBlendState	targetStates[DUEL_MAX_BLEND_LAYERS];
	};

	
	enum TextureAddressMode
	{
		/// Texture wraps at values over 1.0
		TAM_Wrap,
		/// Texture mirrors (flips) at joins over 1.0
		TAM_Mirror,
		/// Texture clamps at 1.0
		TAM_Clamp,
		/// Texture coordinates outside the range [0.0, 1.0] are set to the border colour
		TAM_Border
	};

	// TODO: 
	// this state is seldomly changed, so it is designed for special use and excluded from RenderPass,
	// you can set this state in RenderSystem
	struct DUEL_API DTextureSampler
	{
		// default values are set in constructor-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		DTextureSampler();
		DTextureSampler(const DTextureSampler& tss) { operator=(tss); }
		bool					operator == (const DTextureSampler& tss);
		bool					operator != (const DTextureSampler& tss) { return !operator==(tss); }
		// addresing mode for specified coordinate-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		TextureAddressMode	addressU;			// default: TAM_CLAMP
		TextureAddressMode	addressV;			// default: TAM_CLAMP
		TextureAddressMode	addressW;			// default: TAM_CLAMP
		// minification/magnification filter -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		FilterOption		minFilter;			// default: FO_Linear
		FilterOption		magFilter;			// default: FO_Linear
		// mipmap filter
		FilterOption		mipFilter;			// default: FO_Linear
// 		// A function that compares sampled data against existing sampled data-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 		CompareFunction		samplerComparison;	// default: CF_ALWAYS_FAIL
		// border color, if address mode is set to be TAM_BORDER-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		DColor				borderColor;		// default: DColor::BLACK
		// Lower end of the mipmap range to clamp access to-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		float				minLod;				// default: Math::NEG_INFINITY
		// Upper end of the mipmap range to clamp access to-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		float				maxLod;				// default: Math::POS_INFINITY
		// Clamping value used in FO_ANISOTROPIC, valid value is 0 to 16
		uint32				maxAnisotropy;		// default: 16
		// Offset from the calculated mipmap level-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
		float				mipLodBias;			// default: 0.0f
	};

	// object that used to define a specified render state.
	// here we just make it an interface, different render system should implement it.
	class DUEL_API DRasterizerStateObject : public DObject
	{
	DUEL_DECLARE_RTTI(DRasterizerStateObject)
	public:
		virtual	void	getRasterizerState(DRasterizerState& outState) = 0;
	};

	class DUEL_API DDepthStencilStateObject : public DObject
	{
	DUEL_DECLARE_RTTI(DDepthStencilStateObject)
	public:
		virtual void	getDepthStencilState(DDepthStencilState& outState) = 0;
	};

	class DUEL_API DBlendStateObject : public DObject
	{
	DUEL_DECLARE_RTTI(DBlendStateObject)
	public:
		virtual void	getBlendState(DBlendState& outState) = 0;
	};

	class DUEL_API DTextureSamplerObject : public DObject
	{
	DUEL_DECLARE_RTTI(DTextureSamplerObject)
	public:
		virtual void	getTextureSampler(DTextureSampler& outSamp) = 0;
	};
}

#endif