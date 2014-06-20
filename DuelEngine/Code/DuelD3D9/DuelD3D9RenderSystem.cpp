//  [6/6/2014 OMEGA] created

#include "DuelD3D9Common.h"
#include "DuelD3D9RenderSystem.h"
#include "DuelD3D9RenderState.h"
#include "DuelRenderResourceManager.h"
#include "DuelD3D9RenderResourceFactory.h"
#include "DuelD3D9FrameBuffer.h"
#include "DuelD3D9RenderView.h"
#include "DuelD3D9ShaderObject.h"
#include "DuelD3D9RenderLayout.h"
#include "DuelD3D9IndexBuffer.h"
#include "DuelD3D9VertexBuffer.h"
#include "DuelD3D9FrameBufferCache.h"
#include "DuelD3D9Translator.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(D3D9RenderSystem, DRenderSystem);

	D3D9RenderSystem::D3D9RenderSystem(D3D9RenderResourceFactory* fact) :
		mFactory(fact),
		mName("Direct3D9"),
		mDevice(NULL),
		mCurFrameBuffer(NULL),
		mBlitQuadVert(NULL),
		mBlitQuadIndx(NULL),
		mBlitVDecl(NULL),
		mBlitVShader(NULL),
		mBlitPShader(NULL)
	{

	}

	const DString& D3D9RenderSystem::getName() const
	{
		return mName;
	}

	void D3D9RenderSystem::shutdown()
	{
		ReleaseCOM(mBlitQuadVert);
		ReleaseCOM(mBlitQuadIndx);
		ReleaseCOM(mBlitVDecl);
		ReleaseCOM(mBlitVShader);
		ReleaseCOM(mBlitPShader);
		mDevice = NULL;
		mFactory = NULL;

	}

	void D3D9RenderSystem::bindFrameBuffer( DFrameBuffer* buf )
	{
		mCurFrameBuffer = buf;
		if (buf == NULL)
		{
			return;
		}
		for (uint32 i = EA_Color0; i <= EA_Color7; ++i)
		{
			DRenderColorView* v = buf->getRenderColorView((ElementAttachment)i);
			if(v != NULL)
			{
				D3D9RenderColorView* d3dView = v->getAs<D3D9RenderColorView>();
				IDirect3DSurface9* suf = d3dView->getRenderSurface();
				mDevice->SetRenderTarget(i, suf);
			}
		}
		DRenderDepthStencilView* dsView = buf->getRenderDepthStencilView();
		if (dsView != NULL)
		{
			D3D9RenderDepthStencilView* d3dDSView = dsView->getAs<D3D9RenderDepthStencilView>();
			mDevice->SetDepthStencilSurface(d3dDSView->getRenderSurface());
		}
		else
		{
			mDevice->SetDepthStencilSurface(NULL);
		}
		DViewport vp = buf->getViewport();
		D3DVIEWPORT9 d3dvp;
		d3dvp.X = vp.getLeft();
		d3dvp.Y = vp.getTop();
		d3dvp.Width = vp.getWidth();
		d3dvp.Height = vp.getHeight();
		d3dvp.MinZ = 0.0f;
		d3dvp.MaxZ = 1.0f;
		mDevice->SetViewport(&d3dvp);
	}

	DFrameBuffer* D3D9RenderSystem::getCurrentFrameBuffer()
	{
		return mCurFrameBuffer;
	}

	void D3D9RenderSystem::setRasterizerState( DRasterizerStateObject* rs )
	{
		if (rs == NULL)
		{
			return;
		}		
		D3D9RasterizerStateObject* stateObj = rs->getAs<D3D9RasterizerStateObject>(true);
		DRasterizerState newState;
		stateObj->getRasterizerState(newState);
		if ( mCurRasState.polygonMode != newState.polygonMode)
		{
			mDevice->SetRenderState(D3DRS_FILLMODE, stateObj->D3DPolygonMode);
		}
		if ( mCurRasState.shadeMode != newState.shadeMode)
		{
			mDevice->SetRenderState(D3DRS_SHADEMODE, stateObj->D3DShadeMode);			
		}
		if (mCurRasState.cullingMode != newState.cullingMode)
		{
			mDevice->SetRenderState(D3DRS_SHADEMODE, stateObj->D3DShadeMode);
		}
		if (mCurRasState.polygonOffsetFactor != newState.polygonOffsetFactor)
		{
			mDevice->SetRenderState(D3DRS_DEPTHBIAS, (DWORD)newState.polygonOffsetFactor);
		}
		if (mCurRasState.polygonOffsetUnit != newState.polygonOffsetUnit)
		{
			mDevice->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, (DWORD)newState.polygonOffsetUnit);
		}
		if (mCurRasState.scissorEnable != newState.scissorEnable)
		{
			mDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, (DWORD)newState.scissorEnable);
		}
		if (mCurRasState.multisampleEnable != newState.multisampleEnable)
		{
			mDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, (DWORD)newState.multisampleEnable);
		}
		mCurRasState = newState;
	}

	void D3D9RenderSystem::setDepthStencilState( DDepthStencilStateObject* dss, uint32 frontStencilRef /*= 0*/, uint32 backStencilRef /*= 0 */ )
	{
		if (dss == NULL)
		{
			return;
		}		
		D3D9DepthStencilStateObject* stateObj = dss->getAs<D3D9DepthStencilStateObject>(true);
		DDepthStencilState newState;
		stateObj->getDepthStencilState(newState);
		if (mCurDepState.depthEnable != newState.depthEnable)
		{
			mDevice->SetRenderState(D3DRS_ZENABLE, (DWORD)newState.depthEnable);
		}
		if (mCurDepState.depthWriteEnable != newState.depthWriteEnable)
		{
			mDevice->SetRenderState(D3DRS_ZWRITEENABLE, (DWORD)newState.depthWriteEnable);
		}
		if (mCurDepState.depthComparison != newState.depthComparison)
		{
			mDevice->SetRenderState(D3DRS_ZFUNC, stateObj->D3DDepthComparison);
		}
		// front stencil
		if (mCurDepState.frontStencilEnable != newState.frontStencilEnable)
		{
			mDevice->SetRenderState(D3DRS_STENCILENABLE, (DWORD)newState.frontStencilEnable);
		}
		if (mCurDepState.frontStencilComparison != newState.frontStencilComparison)
		{
			mDevice->SetRenderState(D3DRS_STENCILFUNC, stateObj->D3DFrontStencilComparison);
		}
		if (mCurDepState.frontStencilReadMask != newState.frontStencilReadMask)
		{
			mDevice->SetRenderState(D3DRS_STENCILMASK, newState.frontStencilReadMask);
		}
		if (mCurDepState.frontStencilWriteMask != newState.frontStencilWriteMask)
		{
			mDevice->SetRenderState(D3DRS_STENCILWRITEMASK, newState.frontStencilWriteMask);
		}
		if (mCurDepState.frontStencilFail != newState.frontStencilFail)
		{
			mDevice->SetRenderState(D3DRS_STENCILFAIL, stateObj->D3DFrontStencilFail);
		}
		if (mCurDepState.frontStencilDepthFail != newState.frontStencilDepthFail)
		{
			mDevice->SetRenderState(D3DRS_STENCILZFAIL, stateObj->D3DFrontStencilDepthFail);
		}
		if (mCurDepState.frontStencilPass != newState.frontStencilPass)
		{
			mDevice->SetRenderState(D3DRS_STENCILPASS, stateObj->D3DFrontStencilPass);
		}
		// back stencil
		if (mCurDepState.backStencilEnable != newState.backStencilEnable)
		{
			mDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, (DWORD)newState.backStencilEnable);
		}
		if (mCurDepState.backStencilComparison != newState.backStencilComparison)
		{
			mDevice->SetRenderState(D3DRS_CCW_STENCILFUNC, stateObj->D3DBackStencilComparison);
		}

		// dx9 only support font stencil read/write mask.
		// 		if (mCurDepState.backStencilReadMask != newState.backStencilReadMask)
		// 		{
		// 			mDevice->SetRenderState(D3DRS_STENCILMASK, newState.backStencilReadMask);
		// 		}
		// 		if (mCurDepState.backStencilWriteMask != newState.backStencilWriteMask)
		// 		{
		// 			mDevice->SetRenderState(D3DRS_STENCILWRITEMASK, newState.backStencilWriteMask);
		// 		}
		if (mCurDepState.backStencilFail != newState.backStencilFail)
		{
			mDevice->SetRenderState(D3DRS_CCW_STENCILFAIL, stateObj->D3DBackStencilFail);
		}
		if (mCurDepState.backStencilDepthFail != newState.backStencilDepthFail)
		{
			mDevice->SetRenderState(D3DRS_CCW_STENCILZFAIL, stateObj->D3DBackStencilDepthFail);
		}
		if (mCurDepState.backStencilPass != newState.backStencilPass)
		{
			mDevice->SetRenderState(D3DRS_CCW_STENCILPASS, stateObj->D3DBackStencilPass);
		}

		mCurDepState = newState;
	}

	void D3D9RenderSystem::setBlendState( DBlendStateObject* sbs, const DColor& blendFactor )
	{
		if (sbs == NULL)
		{
			return;
		}		
		D3D9BlendStateObject* stateObj = sbs->getAs<D3D9BlendStateObject>(true);
		DBlendState newState;
		stateObj->getBlendState(newState);

		if (mCurBlendState.alphaToCoverageEnable != newState.alphaToCoverageEnable)
		{
			if (newState.alphaToCoverageEnable)
			{
				mDevice->SetRenderState(D3DRS_POINTSIZE, (MAKEFOURCC('A','2','M','1')));
			}
			else
			{
				mDevice->SetRenderState(D3DRS_POINTSIZE, (MAKEFOURCC('A','2','M','0')));
			}
		}
		// dx9 only support single blend function.
		if (mCurBlendState.targetStates[0].blendEnable != newState.targetStates[0].blendEnable)
		{
			mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, (DWORD)newState.targetStates[0].blendEnable);
		}
		// color
		if (mCurBlendState.targetStates[0].colorBlendOpeartion != newState.targetStates[0].colorBlendOpeartion)
		{
			mDevice->SetRenderState(D3DRS_BLENDOP, stateObj->D3DColorBlendOperation[0]);
		}
		if (mCurBlendState.targetStates[0].colorSrcFactor != newState.targetStates[0].colorSrcFactor)
		{
			mDevice->SetRenderState(D3DRS_SRCBLEND, stateObj->D3DColorSrcFactor[0]);
		}
		if (mCurBlendState.targetStates[0].colorDstFactor != newState.targetStates[0].colorDstFactor)
		{
			mDevice->SetRenderState(D3DRS_DESTBLEND, stateObj->D3DColorDstFactor[0]);
		}
		// for alpha 
		if (mCurBlendState.targetStates[0].alphaBlendOpeartion != newState.targetStates[0].alphaBlendOpeartion)
		{
			mDevice->SetRenderState(D3DRS_BLENDOPALPHA, stateObj->D3DAlphaBlendOperation[0]);
		}
		if (mCurBlendState.targetStates[0].alphaSrcFactor != newState.targetStates[0].alphaSrcFactor)
		{
			mDevice->SetRenderState(D3DRS_SRCBLENDALPHA, stateObj->D3DAlphaSrcFactor[0]);
		}
		if (mCurBlendState.targetStates[0].alphaDstFactor != newState.targetStates[0].alphaDstFactor)
		{
			mDevice->SetRenderState(D3DRS_DESTBLENDALPHA, stateObj->D3DAlphaDstFactor[0]);
		}

		mCurBlendState = newState;
	}

	Duel::DRasterizerStateObjectPtr D3D9RenderSystem::getCurrentRaseterizerState()
	{
		return DRenderResourceManager::getSingleton().createRasterizerStateObject(mCurRasState);
	}

	Duel::DDepthStencilStateObjectPtr D3D9RenderSystem::getCurrentDepthStencilStateObject()
	{
		return DRenderResourceManager::getSingleton().createDepthStencilStateObject(mCurDepState);
	}

	Duel::DBlendStateObjectPtr D3D9RenderSystem::getCurrentBlendState()
	{
		return DRenderResourceManager::getSingleton().createBlendStateObject(mCurBlendState);
	}

	void D3D9RenderSystem::bindShaderObject( DShaderObject* so )
	{
		if (so == NULL || !so->isValid())
		{
			return;
		}
		D3D9ShaderObject* d3dShaderObj = so->getAs<D3D9ShaderObject>();
		mDevice->SetVertexShader(d3dShaderObj->getD3DVertexShader());
		mDevice->SetPixelShader(d3dShaderObj->getD3DPixelShader());

		DGpuParameters* vsParam = d3dShaderObj->getVertexProgramParameters();
		DGpuParameters* psParam = d3dShaderObj->getPixelProgramParameters();

		// vs parameter.
		setVertexShaderParameter(mDevice, vsParam);
		setPixelShaderParameter(mDevice, psParam);
	}

	void D3D9RenderSystem::render( DRenderLayout* layout )
	{
		if (layout == NULL)
		{
			return;
		}
		DIndexData idata = layout->getIndexData();
		if (idata.getIndexBuffer() == NULL)
		{
			return;
		}
		D3D9RenderLayout* d3dLayout = layout->getAs<D3D9RenderLayout>();
		mDevice->SetVertexDeclaration(d3dLayout->getD3DVertexDeclaration());
		mDevice->SetIndices(idata.getIndexBuffer()->getAs<D3D9IndexBuffer>()->getD3DIndexBuffer());
		DVertexStreams vdata = layout->getVertexData().getBufferStreams();
		DVertexStreams::VertexStreamIterator vi = vdata.getVertexStreamIterator();
		while (vi.hasMoreElements())
		{
			uint32 idx = vi.peekNextKey();
			D3D9VertexBuffer* vBuf = vi.getNext()->getAs<D3D9VertexBuffer>();
			mDevice->SetStreamSource(idx, vBuf->getD3DVertexBuffer(), 0, vBuf->getVertexSize());
			// do not supprot instanced-drawing now.
		}
		uint32 pDivider = 1;
		switch (d3dLayout->getTopologyType())
		{
		case PT_PointList:
			pDivider = 1;
			break;
		case PT_LineList:
		case PT_LineStrip:
			pDivider = 2;
			break;
		case PT_TriangleFan:
		case PT_TriangleList:
		case PT_TriangleStrip:
			pDivider = 3;
			break;
		}
		mDevice->DrawPrimitive(d3dLayout->getD3DPrimitiveType(), idata.getIndexStart(), 
			(idata.getIndexEnd() - idata.getIndexStart())/pDivider);
	}

	void D3D9RenderSystem::fillDeviceCaps()
	{
		
	}

	void D3D9RenderSystem::initialize()
	{

		// initialize render states
		// with an non-default value to force it to init with default value.
		mCurRasState.polygonMode = PM_Wireframe;
		mCurRasState.shadeMode = SM_Flat;
		mCurRasState.cullingMode = CM_None;
		mCurRasState.polygonOffsetFactor = 1.0f;
		mCurRasState.polygonOffsetUnit = 1.0f;
		mCurRasState.scissorEnable = true;
		mCurRasState.multisampleEnable = false;

		mCurDepState.depthEnable = false;
		mCurDepState.depthWriteEnable = false;
		mCurDepState.depthComparison = CF_Greater;
		mCurDepState.frontStencilEnable = true;
		mCurDepState.frontStencilComparison = CF_Less;
		mCurDepState.frontStencilReadMask = 0;
		mCurDepState.frontStencilWriteMask = 0;
		mCurDepState.frontStencilFail = SO_Decrement;
		mCurDepState.frontStencilDepthFail = SO_Decrement;
		mCurDepState.frontStencilPass = SO_Decrement;
		mCurDepState.backStencilEnable = true;
		mCurDepState.backStencilComparison = CF_Less;
		mCurDepState.backStencilReadMask = 0;
		mCurDepState.backStencilWriteMask = 0;
		mCurDepState.backStencilFail = SO_Decrement;
		mCurDepState.backStencilDepthFail = SO_Decrement;
		mCurDepState.backStencilPass = SO_Decrement;

		mCurBlendState.alphaToCoverageEnable = false;
		for (uint32 i = 0; i < 8; ++i)
		{
			mCurBlendState.targetStates[i].blendEnable = true;
			mCurBlendState.targetStates[i].colorBlendOpeartion = BO_Sub;
			mCurBlendState.targetStates[i].colorSrcFactor = BF_Zero;
			mCurBlendState.targetStates[i].colorDstFactor = BF_One;

			mCurBlendState.targetStates[i].alphaBlendOpeartion = BO_Sub;
			mCurBlendState.targetStates[i].alphaSrcFactor = BF_Zero;
			mCurBlendState.targetStates[i].alphaDstFactor = BF_One;
			mCurBlendState.targetStates[i].colorWriteMask = 0;
		}
		mCurBlendFactor = DColor::WHITE;

		mDevice = mFactory->getMainDevice();
		mDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);

		fillDeviceCaps();
		initRenderStates();


		// create resources for transfering data from render target to 
		// the back buffer.
		D3DVERTEXELEMENT9 dwDecl3[] = 
		{
			{0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, 
			D3DDECLUSAGE_POSITION, 0},
			{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, 
			D3DDECLUSAGE_TEXCOORD, 0},
			D3DDECL_END()
		};
		mDevice->CreateVertexDeclaration(dwDecl3, &mBlitVDecl);
		// a quad.
		float vertData[] =
		{ 
			/*vert*/ -1.0f, 1.0f, 0.0f, /*uv*/ 0.0f, 0.0f,
			/*vert*/ 1.0f,  1.0f, 0.0f, /*uv*/ 1.0f, 0.0f,
			/*vert*/ 1.0f, -1.0f, 0.0f, /*uv*/ 1.0f, 1.0f,
			/*vert*/ -1.0f,-1.0f, 0.0f, /*uv*/ 0.0f, 1.0f
		};
		mDevice->CreateVertexBuffer(sizeof(float)*20, 0, 0, D3DPOOL_MANAGED, &mBlitQuadVert, NULL);
		void* tempBuf = NULL;
		mBlitQuadVert->Lock(0,sizeof(float)*20, &tempBuf, D3DLOCK_DISCARD);
		memcpy(tempBuf, vertData, sizeof(float)*20);
		mBlitQuadVert->Unlock();
		int16 idata[] =
		{
			0, 3, 2,  0, 2, 1
		};
		mDevice->CreateIndexBuffer(sizeof(uint16)*6, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &mBlitQuadIndx, NULL);
		mBlitQuadIndx->Lock(0, sizeof(uint16)*6, &tempBuf, D3DLOCK_DISCARD);
		memcpy(tempBuf, idata, sizeof(uint16)*6);
		mBlitQuadIndx->Unlock();
		DString vsCode = "struct VS_INPUT\
						 {\
						 float4 Position   : POSITION;\
						 float2 Texcoord   : TEXCOORD0;\
						 };\
						 struct VS_OUTPUT \
						 {\
						 float4 Position   : POSITION;\
						 float2 Texcoord   : TEXCOORD0;\
						 };\
						 VS_OUTPUT main(in VS_INPUT Input)\
						 {\
						 VS_OUTPUT Output; Output.Position = Input.Position; Output.Texcoord = Input.Texcoord; return Output;\
						 }";
		ID3DXBuffer* vsCodeBuf;
		D3DXCompileShader(vsCode.c_str(), vsCode.length(),NULL,NULL,"main","vs_2_0",0, &vsCodeBuf, NULL, NULL);
		HRESULT hr = mDevice->CreateVertexShader(static_cast<DWORD*>(vsCodeBuf->GetBufferPointer()), &mBlitVShader);
		vsCodeBuf->Release();

		DString psCode = "struct PS_INPUT\
						 {\
						 float4 Position   : POSITION;\
						 float2 Texcoord   : TEXCOORD0;\
						 };\
						 sampler srcTex : register(s0);\
						 float4 main(in PS_INPUT input) : COLOR0\
						 {\
						 return tex2D(srcTex, input.Texcoord);\
						 }\
						 ";
		ID3DXBuffer* psCodeBuf;
		D3DXCompileShader(psCode.c_str(), psCode.length(),NULL,NULL,"main","ps_2_0",0, &psCodeBuf, NULL, NULL);
		hr = mDevice->CreatePixelShader(static_cast<DWORD*>(psCodeBuf->GetBufferPointer()), &mBlitPShader);
		psCodeBuf->Release();

		DRasterizerState rasState;
		rasState.cullingMode = CM_ClockWise;
		rasState.polygonMode = PM_Solid;
		rasState.multisampleEnable = false;
		rasState.shadeMode = SM_Flat;
		mBlitRasState = DRenderResourceManager::getSingleton().createRasterizerStateObject(rasState);

		DDepthStencilState depState;
		depState.depthEnable = false;
		mBlitDepthState = DRenderResourceManager::getSingleton().createDepthStencilStateObject(depState);

		DBlendState blendState;
		blendState.targetStates[0].blendEnable = false;
		mBlitBlendState = DRenderResourceManager::getSingleton().createBlendStateObject(blendState);
	}

	void D3D9RenderSystem::initRenderStates()
	{
		DRenderResourceManager* resMgr = DRenderResourceManager::getSingletonPtr();
		DRasterizerStateObjectPtr rasPtr = resMgr->createRasterizerStateObject(mCurRasState);
		DDepthStencilStateObjectPtr depPtr = resMgr->createDepthStencilStateObject(mCurDepState);
		DBlendStateObjectPtr blendPtr = resMgr->createBlendStateObject(mCurBlendState);

		setRasterizerState(rasPtr.get());
		setDepthStencilState(depPtr.get());
		setBlendState(blendPtr.get(), DColor(0.0f, 0.0f, 0.0f, 0.0f));

	}

	void D3D9RenderSystem::setVertexShaderParameter( IDirect3DDevice9* dev, DGpuParameters* vsParam )
	{
		DGpuParameters::GpuConstantIterator ci = vsParam->getAutoGpuConstantIterator();
		while (ci.hasMoreElements())
		{
			DString paramName = ci.peekNextKey();
			GpuConstantDefinition constdef = ci.getNext();
			// HLSL always packing.
			assert(constdef.elementSize % 4 == 0);
			if (constdef.isBool())
			{
				dev->SetVertexShaderConstantB(constdef.logicalIndex,
					vsParam->getBoolValuePtr(constdef.physicalIndex),
					constdef.elementSize/4);
			}
			else if (constdef.isInt())
			{
				dev->SetVertexShaderConstantI(constdef.logicalIndex,
					vsParam->getIntValuePtr(constdef.physicalIndex),
					constdef.elementSize/4);
			}
			else if (constdef.isFloat())
			{
				dev->SetVertexShaderConstantF(constdef.logicalIndex,
					vsParam->getFloatValuePtr(constdef.physicalIndex),
					constdef.elementSize/4);
			}
			else if (constdef.isTexture())
			{
				DGpuTextureConstantPtr tex = vsParam->getTextureConstant(paramName);
				setTextureSampler(dev, constdef.logicalIndex, tex);
			}
		}
		ci = vsParam->getCustomGpuConstantIterator();
		while (ci.hasMoreElements())
		{
			DString paramName = ci.peekNextKey();
			GpuConstantDefinition constdef = ci.getNext();
			// HLSL always packing.
			assert(constdef.elementSize % 4 == 0);
			if (constdef.isBool())
			{
				dev->SetVertexShaderConstantB(constdef.logicalIndex,
					vsParam->getBoolValuePtr(constdef.physicalIndex),
					constdef.elementSize/4);
			}
			else if (constdef.isInt())
			{
				dev->SetVertexShaderConstantI(constdef.logicalIndex,
					vsParam->getIntValuePtr(constdef.physicalIndex),
					constdef.elementSize/4);
			}
			else if (constdef.isFloat())
			{
				dev->SetVertexShaderConstantF(constdef.logicalIndex,
					vsParam->getFloatValuePtr(constdef.physicalIndex),
					constdef.elementSize/4);
			}
			else if (constdef.isTexture())
			{
				DGpuTextureConstantPtr tex = vsParam->getTextureConstant(paramName);
				setTextureSampler(dev, constdef.logicalIndex, tex);
			}
		}
	}

	void D3D9RenderSystem::setPixelShaderParameter( IDirect3DDevice9* dev, DGpuParameters* psParam )
	{
		DGpuParameters::GpuConstantIterator ci = psParam->getAutoGpuConstantIterator();
		while (ci.hasMoreElements())
		{
			DString paramName = ci.peekNextKey();
			GpuConstantDefinition constdef = ci.getNext();
			// HLSL always packing.
			assert(constdef.elementSize % 4 == 0);
			if (constdef.isBool())
			{
				dev->SetVertexShaderConstantB(constdef.logicalIndex,
					psParam->getBoolValuePtr(constdef.physicalIndex),
					constdef.elementSize/4);
			}
			else if (constdef.isInt())
			{
				dev->SetVertexShaderConstantI(constdef.logicalIndex,
					psParam->getIntValuePtr(constdef.physicalIndex),
					constdef.elementSize/4);
			}
			else if (constdef.isFloat())
			{
				dev->SetVertexShaderConstantF(constdef.logicalIndex,
					psParam->getFloatValuePtr(constdef.physicalIndex),
					constdef.elementSize/4);
			}
			else if (constdef.isTexture())
			{
				DGpuTextureConstantPtr tex = psParam->getTextureConstant(paramName);
				setTextureSampler(dev, constdef.logicalIndex, tex);
			}
		}
		ci = psParam->getCustomGpuConstantIterator();
		while (ci.hasMoreElements())
		{
			DString paramName = ci.peekNextKey();
			GpuConstantDefinition constdef = ci.getNext();
			// HLSL always packing.
			assert(constdef.elementSize % 4 == 0);
			if (constdef.isBool())
			{
				dev->SetVertexShaderConstantB(constdef.logicalIndex,
					psParam->getBoolValuePtr(constdef.physicalIndex),
					constdef.elementSize/4);
			}
			else if (constdef.isInt())
			{
				dev->SetVertexShaderConstantI(constdef.logicalIndex,
					psParam->getIntValuePtr(constdef.physicalIndex),
					constdef.elementSize/4);
			}
			else if (constdef.isFloat())
			{
				dev->SetVertexShaderConstantF(constdef.logicalIndex,
					psParam->getFloatValuePtr(constdef.physicalIndex),
					constdef.elementSize/4);
			}
			else if (constdef.isTexture())
			{
				DGpuTextureConstantPtr tex = psParam->getTextureConstant(paramName);
				setTextureSampler(dev, constdef.logicalIndex, tex);
			}
		}
	}

	void D3D9RenderSystem::setTextureSampler( IDirect3DDevice9* dev, uint32 logicalIdx, DGpuTextureConstantPtr tex )
	{
		if (tex != NULL)
		{
			D3D9GpuTextureConstant* d3dTex = tex->getAs<D3D9GpuTextureConstant>();
			dev->SetTexture(logicalIdx,
				d3dTex->getD3DTexture());
			DTextureSamplerObjectPtr samp = d3dTex->getSampler();
			if (samp != NULL)
			{
				D3D9TextureSamplerObject* d3dSamp = samp->getAs<D3D9TextureSamplerObject>();
				dev->SetSamplerState(logicalIdx,
					D3DSAMP_ADDRESSU, d3dSamp->D3DTextureAddressU);
				dev->SetSamplerState(logicalIdx,
					D3DSAMP_ADDRESSV, d3dSamp->D3DTextureAddressV);
				dev->SetSamplerState(logicalIdx,
					D3DSAMP_ADDRESSW, d3dSamp->D3DTextureAddressW);
				dev->SetSamplerState(logicalIdx,
					D3DSAMP_BORDERCOLOR, d3dSamp->D3DBorderColor);
				dev->SetSamplerState(logicalIdx,
					D3DSAMP_MAGFILTER, d3dSamp->D3DMagFilter);
				dev->SetSamplerState(logicalIdx,
					D3DSAMP_MINFILTER, d3dSamp->D3DMinFilter);
				dev->SetSamplerState(logicalIdx,
					D3DSAMP_MIPFILTER, d3dSamp->D3DMipFilter);
				int32 iLodBias = (int32)d3dSamp->mPreserveSamp.mipLodBias;
				dev->SetSamplerState(logicalIdx,
					D3DSAMP_MIPMAPLODBIAS, iLodBias);
				// don't handle for min/max lod.
				dev->SetSamplerState(logicalIdx,
					D3DSAMP_MAXANISOTROPY, d3dSamp->mPreserveSamp.maxAnisotropy);
			}
		}
	}

	void D3D9RenderSystem::clearFrameBuffer( DFrameBuffer* fb, uint32 flags, const DColor& clr, DReal depth, int32 stencil )
	{
		D3D9FrameBufferCache cache(this, mCurFrameBuffer);
		bindFrameBuffer(fb);

		DWORD  d3dClearFlag = 0;
		if (flags & CBM_Color)
		{
			d3dClearFlag |= D3DCLEAR_TARGET;
		}
		if ((flags & CBM_Depth) || (flags & CBM_Stencil))
		{
			d3dClearFlag |= D3DCLEAR_ZBUFFER;
			d3dClearFlag |= D3DCLEAR_STENCIL;
		}
		D3DCOLOR d3dColor = D3D9Translator::getD3DColor(clr);
		mDevice->Clear(0, NULL, d3dClearFlag, d3dColor, depth, stencil);
	}

	void D3D9RenderSystem::blitTexture( IDirect3DBaseTexture9* input, IDirect3DSurface9* output, DViewport outputVP )
	{
		D3D9FrameBufferCache cache(this, mCurFrameBuffer);
		HRESULT hr = mDevice->SetRenderTarget(0, output);
		hr = mDevice->SetDepthStencilSurface(NULL);
		hr = mDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3D9Translator::getD3DColor(DColor::BLACK), 0.0f, 0);
		D3DVIEWPORT9 d3dvp;
		d3dvp.X = outputVP.getLeft();
		d3dvp.Y = outputVP.getTop();
		d3dvp.Width = outputVP.getWidth();
		d3dvp.Height = outputVP.getHeight();
		d3dvp.MinZ = 0.0f;
		d3dvp.MaxZ = 1.0f;
		mDevice->SetViewport(&d3dvp);
		mDevice->SetIndices(mBlitQuadIndx);
		mDevice->SetStreamSource(0,mBlitQuadVert,0,sizeof(float)*5);
		mDevice->SetVertexDeclaration(mBlitVDecl);
		mDevice->SetTexture(0, input);

		setRasterizerState(mBlitRasState.get());
		setDepthStencilState(mBlitDepthState.get());
		setBlendState(mBlitBlendState.get(), DColor::WHITE);

		hr = mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	}

}