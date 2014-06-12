//  [6/6/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9ShaderObject.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9ShaderObject, DShaderObject);

	D3D9ShaderObject::D3D9ShaderObject()
	{

	}

	DGpuParameters* D3D9ShaderObject::getVertexProgramParameters()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	DGpuParameters* D3D9ShaderObject::getPixelProgramParameters()
	{
		throw std::exception("The method or operation is not implemented.");
	}

}