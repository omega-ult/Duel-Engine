//  [6/6/2014 OMEGA] created

#ifndef _DUELD3D9SHADEROBJECT_H_
#define _DUELD3D9SHADEROBJECT_H_

#include "DuelD3D9Common.h"
#include "DuelShaderObject.h"

namespace Duel
{
	class D3D9ShaderObject : public DShaderObject
	{
		DUEL_DECLARE_RTTI(D3D9ShaderObject)
	public:
		D3D9ShaderObject();

		virtual DGpuParameters* getVertexProgramParameters();

		virtual DGpuParameters* getPixelProgramParameters();

	};
}

#endif