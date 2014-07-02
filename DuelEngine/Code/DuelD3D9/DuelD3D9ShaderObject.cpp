//  [6/6/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9ShaderObject.h"
#include "DuelRenderPass.h"
#include "DuelD3D9GpuProgram.h"
#include "DuelD3D9RenderResourceFactory.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9GpuTextureConstant, DGpuTextureConstant);
	DUEL_IMPLEMENT_RTTI_1(D3D9ShaderObject, DShaderObject);
	// utility function, map D3DXCONSTANT to GpuConstantDefinition.
	void getDefinition(D3DXCONSTANT_DESC& d3dConstDesc, GpuConstantDefinition& def)
	{
		def.arraySize = d3dConstDesc.Elements;
		switch(d3dConstDesc.Type)
		{
		case D3DXPT_BOOL:
			def.constType = GCT_Bool;
			break;
		case D3DXPT_INT:
			switch(d3dConstDesc.Columns)
			{
			case 1:
				def.constType = GCT_Int1;
				break;
			case 2:
				def.constType = GCT_Int2;
				break;
			case 3:
				def.constType = GCT_Int3;
				break;
			case 4:
				def.constType = GCT_Int4;
				break;
			}
			break;
		case D3DXPT_FLOAT:
			switch(d3dConstDesc.Class)
			{
				// we set the matrix packing in compilation, so here the two
				// packing will be regarded as the same.
			case D3DXPC_MATRIX_COLUMNS:
			case D3DXPC_MATRIX_ROWS:
				{
					int firstDim, secondDim;
					firstDim = d3dConstDesc.RegisterCount / d3dConstDesc.Elements;
					if (d3dConstDesc.Class == D3DXPC_MATRIX_ROWS)
					{
						secondDim = d3dConstDesc.Columns;
					}
					else
					{
						secondDim = d3dConstDesc.Rows;
					}
					switch(firstDim)
					{
					case 2:
						switch(secondDim)
						{
						case 2:
							def.constType = GCT_Matrix2x2;
							break;
						case 3:
							def.constType = GCT_Matrix2x3;
							break;
						case 4:
							def.constType = GCT_Matrix2x4;
							break;
						}
						break;
					case 3:
						switch(secondDim)
						{
						case 2:
							def.constType = GCT_Matrix3x2;
							break;
						case 3:
							def.constType = GCT_Matrix3x3;
							break;
						case 4:
							def.constType = GCT_Matrix3x4;
							break;
						}
						break;
					case 4:
						switch(secondDim)
						{
						case 2:
							def.constType = GCT_Matrix4x2;
							break;
						case 3:
							def.constType = GCT_Matrix4x3;
							break;
						case 4:
							def.constType = GCT_Matrix4x4;
							break;
						} // secondDim
						break;
					} // firstDim
				}
				break;
			case D3DXPC_SCALAR:
			case D3DXPC_VECTOR:
				switch(d3dConstDesc.Columns)
				{
				case 1:
					def.constType = GCT_Float1;
					break;
				case 2:
					def.constType = GCT_Float2;
					break;
				case 3:
					def.constType = GCT_Float3;
					break;
				case 4:
					def.constType = GCT_Float4;
					break;
				}
				break;
			}
		default:
			// not mapping samplers, don't need to take the space 
			break;
		} // switch(d3dConstDesc.Type)

		def.elementSize = GpuConstantDefinition::getElementSize(def.constType, true);
	}

	D3D9ShaderObject::D3D9ShaderObject(D3D9RenderResourceFactory* fact) :
		mCreator(fact),
		mVShader(NULL),
		mPShader(NULL),
		mVSParameter(""),
		mPSParameter("")
	{

	}

	DGpuParameters* D3D9ShaderObject::getVertexProgramParameters()
	{
		return &mVSParameter;
	}

	DGpuParameters* D3D9ShaderObject::getPixelProgramParameters()
	{
		return &mPSParameter;
	}

	void D3D9ShaderObject::build( DRenderPass* pass )
	{
		if (pass == NULL)
		{
			return;
		}
		if ((pass->getVertexProgram() == NULL && !pass->getVertexProgram()->isLoaded())
			|| (pass->getPixelProgram() == NULL && !pass->getPixelProgram()->isLoaded()))
		{
			return;
		}
		DShaderObject::build(pass);

		ReleaseCOM(mVShader);
		ReleaseCOM(mPShader);

		D3D9GpuProgram* vs = pass->getVertexProgram()->getAs<D3D9GpuProgram>();
		D3D9GpuProgram* ps = pass->getPixelProgram()->getAs<D3D9GpuProgram>();
		if (vs->hasCompileError() || ps->hasCompileError())
		{
			std::cout<< vs->getName() + " : " + vs->getCompileError() << std::endl;
			std::cout<< ps->getName() + " : " + ps->getCompileError() << std::endl;

			return;
		}
		mCreator->getMainDevice()->CreateVertexShader(
			static_cast<DWORD*>(vs->getCodeBuffer()->GetBufferPointer()), &mVShader);
		mCreator->getMainDevice()->CreatePixelShader(
			static_cast<DWORD*>(ps->getCodeBuffer()->GetBufferPointer()), &mPShader);
		
		D3DXCONSTANTTABLE_DESC desc;

		ID3DXConstantTable* VSConstTable = vs->getConstantTable();
		VSConstTable->GetDesc(&desc);
		mVSParameter.removeAllConstants();
		mVSParameter.setName(pass->getName() + "." + vs->getName());
		for (uint32 i = 0; i < desc.Constants; ++i)
		{
			// Recursively descend through the structure levels
			processParam(mVSParameter, VSConstTable, NULL, "", i);
		}

		ID3DXConstantTable* PSConstTable = ps->getConstantTable();
		PSConstTable->GetDesc(&desc);
		mPSParameter.removeAllConstants();
		mPSParameter.setName(pass->getName() + "." + ps->getName());
		for (uint32 i = 0; i < desc.Constants; ++i)
		{
			// Recursively descend through the structure levels
			processParam(mPSParameter, PSConstTable, NULL, "", i);
		}
	}

	void D3D9ShaderObject::processParam( DGpuParameters& param, ID3DXConstantTable* constTable, D3DXHANDLE parent, DString prefix, uint32 index )
	{
		D3DXHANDLE hConstant = constTable->GetConstant(parent, index);
		// Since D3D HLSL doesn't deal with naming of array and struct parameters
		// automatically, we have to do it by hand
		D3DXCONSTANT_DESC desc;
		uint32	paramCount = 1;
		if (FAILED(constTable->GetConstantDesc(hConstant, &desc, &paramCount)))
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Can not retrieve constant description",
				"Duel::D3D9ShaderObject::processParam")
		}
		DString paramName = desc.Name;
		// trim the odd '$' which appears at the start of the names in HLSL
		if (paramName.at(0) == '$')
		{
			paramName.erase(paramName.begin());
		}
		// Also trim the '[0]' suffix if it exists, we will add our own indexing later
		if (DStringTool::isEndWith(paramName, "[0]", false))
		{
			paramName.erase(paramName.size() - 3);
		}

		if (desc.Class == D3DXPC_STRUCT)
		{
			// work out a new prefix for nested members, if it's an array, we need an index
			prefix = prefix + paramName + ".";
			// Cascade into struct
			for (uint32 i = 0; i < desc.StructMembers; ++i)
			{
				processParam(param, constTable, hConstant, prefix, i);
			}
		}
		else
		{
			GpuConstantDefinition def;
			// process params
			if (desc.Type == D3DXPT_FLOAT || desc.Type == D3DXPT_INT || desc.Type == D3DXPT_BOOL)
			{
				getDefinition(desc, def);
				def.logicalIndex = desc.RegisterIndex;
			}
			else
			{
				def.logicalIndex = desc.RegisterIndex;
				switch (desc.Type)
				{
				case D3DXPT_SAMPLER1D:
					def.constType = GCT_Texture1D;
					break;
				case D3DXPT_SAMPLER2D:
					def.constType = GCT_Texture2D;
					break;
				case D3DXPT_SAMPLER3D:
					def.constType = GCT_Texture3D;
					break;
				case D3DXPT_SAMPLERCUBE:
					def.constType = GCT_TextureCube;
					break;
				default:
					DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
						"Unknown parameter type",
						"Duel::D3D9ShaderObject::processParam")
					break;
				}
			}
			param.addConstant(paramName,def);
		}
	}

	D3D9GpuTextureConstant::D3D9GpuTextureConstant( IDirect3DBaseTexture9* tex ) :
		mTexture(tex),
		mbValid(true)
	{

	}

	bool D3D9GpuTextureConstant::isValid()
	{
		return mbValid;
	}

	void D3D9GpuTextureConstant::setSamplerObject( DTextureSamplerObjectPtr samp )
	{
		mTexSamp = samp;
	}

}