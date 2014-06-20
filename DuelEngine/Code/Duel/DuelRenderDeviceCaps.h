//  [7/23/2012 OMEGA] created

#ifndef _DUELRENDERDEVICECAPS_H_
#define _DUELRENDERDEVICECAPS_H_

#include "DuelCommon.h"
#include "DuelPixelFormat.h"
#include "DuelString.h"
namespace Duel
{

	struct DisplayMode
	{
		DisplayMode(uint32 w, uint32 h, uint32 rRate, DPixelFormat pf) :
			width(w), height(h), refreshRate(rRate), format(pf) {}

		bool	operator == (const DisplayMode& mode)
		{
			return (width == mode.width &&
					height == mode.height &&
					refreshRate == mode.refreshRate &&
					format == mode.format);
		}
		bool	operator != (const DisplayMode& mode)
		{
			return !operator==(mode);
		}
		uint32		width;
		uint32		height;
		uint32		refreshRate;
		DPixelFormat	format;
	};

	// indicate the capability of the render device
	class DUEL_API RenderDeviceCaps
	{
	public:

		// Maximum texture width for this device. 
		uint32		MaxTextureWidth;
		// Maximum texture height for this device. 
		uint32		MaxTextureHeight;
		// Maximum value for any of the three dimensions (width, height, and depth) of a volume texture. 
		uint32		MaxTextureDepth;


		
		/// The number of texture units available
		// This value has no meaning in the programmable pipeline where the number of sampler 
		// stages is determined by each pixel shader version. Each pixel shader version also 
		// determines the number of texture declaration instructions. See Pixel Shaders.
		uint32		MaxTextureUnits;
		/// The stencil buffer bit depth
		uint32		StencilBufferBitDepth;
		/// The number of matrices available for hardware blending
		uint32		MaxVertexBlendMatrices;

		// Maximum number of primitives
		uint32		MaxPrimitiveCount;
		// Maximum size of indices supported for hardware vertex processing
		uint32		MaxVertexIndex;
		// Maximum number of concurrent data streams 
		uint32		MaxStreams;

		/// The number of floating-point constants vertex shader support
		uint32		VertexProgramConstantFloatCount;  
		/// The number of integer constants vertex shader support
		uint32		VertexProgramConstantIntCount;  
		/// The number of boolean constants vertex shader support
		uint32		VertexProgramConstantBoolCount;
		
		/// The number of floating-point constants geometry shader support
		uint32		GeometryProgramConstantFloatCount;  
		/// The number of integer constants vertex geometry support
		uint32		GeometryProgramConstantIntCount;  
		/// The number of boolean constants vertex geometry support
		uint32		GeometryProgramConstantBoolCount;

		/// The number of floating-point constants pixel shader support
		uint32		PixelProgramConstantFloatCount;           
		/// The number of integer constants pixel shader support
		uint32		PixelProgramConstantIntCount;           
		/// The number of boolean constants pixel shader support
		uint32		PixelProgramConstantBoolCount;
		
		/// The number of simultaneous render targets supported
		uint32		MaxMultiRenderTargets;
		
		// store supported shader profiles
		typedef std::set<DString>			ShaderProfiles;
		ShaderProfiles	SupportedShader;

		typedef	std::vector<DisplayMode>	DisplayModes;
		DisplayModes	SupportedDisplayMode;
		
		void	addSupportDisplayMode(uint32 width, uint32 height, uint32 refreshRate, DPixelFormat format)
		{
			SupportedDisplayMode.push_back(DisplayMode(width, height, refreshRate, format));
		}

		bool	isSupportDisplayMode(const DisplayMode& mode)
		{
			DisplayModes::iterator i, iend = SupportedDisplayMode.end();
			for(i = SupportedDisplayMode.begin(); i != iend; ++i)
			{
				if ((*i) == mode )
				{
					return true;
				}
			}
			return false;
		}

		void	addSupportShader(const DString& s)
		{
			SupportedShader.insert(s);
		}

		bool	isSupportShader(const DString& s)
		{
			ShaderProfiles::iterator i;
			for(i = SupportedShader.begin(); i != SupportedShader.end(); ++i)
			{
				if((*i) == s)
				{
					return true;
				}
			}
			return false;
		}
	};


}

#endif