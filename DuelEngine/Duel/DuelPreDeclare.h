//  [9/16/2012 OMEGA] created


#include <istream>
#include <fstream>
#include <vector>
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
#include <memory>
#else
#include <boost/shared_ptr.hpp>
#endif // DUAL_PLATFORM_WINDOWS_PHONE_8


namespace Duel
{
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
#define SharedPtr	std::shared_ptr
#else
#define	SharedPtr	boost::shared_ptr
#endif // DUEL_PLATFORM_WINDOWS_PHONE_8


	//////////////////////////////////////////////////////////////////////////
	// Predeclaration.
	// a simple declaration that mark specified member function to internal
	// method, which means the method is used in the engine only, and 
	// is 'not recommanded' to be called in client program.
#define	INTERNAL
	//////////////////////////////////////////////////////////////////////////
	// core
	class	DObject;
	class	DCore;
	class	RTTI;
	class	DLog;
	typedef	SharedPtr<DLog>				DLogPtr;
	class	DException;
	class	DLogManager;
	class	DTimer;
	class	DBitTool;
	class	DStringTool;

	typedef	SharedPtr<DStringVector>		DStringVectorPtr;
	//////////////////////////////////////////////////////////////////////////
	// resources
	class	DArchive;
	typedef	SharedPtr<DArchive>				DArchivePtr;
	class	DFolderArchive;
	typedef	SharedPtr<DFolderArchive>		DFolderArchivePtr;

	class	DResource;
	typedef	SharedPtr<DResource>			DResourcePtr;
	class	DResourceDescription;
	typedef	SharedPtr<DResourceDescription>	DResourceDescriptionPtr;
	class	DResourceManager;

	class	DFont;
	class	DFontManager;

	class	DGpuTextureConstant;
	class	DGpuProgram;
	class	DGpuParameters;
	class	DGpuProgramManager;


	class	DTexture;
	class	DTextureManager;

	class	DMesh;
	class	DSubMesh;
	typedef	SharedPtr<DSubMesh>				DSubMeshPtr;
	class	DMeshManager;

	class	DDataStream;
	typedef	SharedPtr<DDataStream>			DDataStreamPtr;
	class	DFileStream;
	typedef	SharedPtr<DFileStream>			DFileStreamPtr;
	class	DMemoryStream;
	typedef	SharedPtr<DMemoryStream>		DMemoryStreamPtr;

	class	DMaterial;
	typedef	SharedPtr<DMaterial>			DMaterialPtr;
	class	DMaterialInstance;
	typedef	SharedPtr<DMaterialInstance>	DMaterialInstancePtr;
	class	DMaterialManager;

	class	DImage;
	typedef SharedPtr<DImage>				DImagePtr;
	class	DImage2D;
	class	DImage3D;
	class	DImageCube;

	class	DMediaDesc;
	class	DImageDesc;

	class	DMediaCodec;
	class	DResourceCodec;
	class	DMCodec_v1_0;
	class	DSCodec_v1_0;
	class	DRECodec_v1_0;
	class	DDSCodec;
	//////////////////////////////////////////////////////////////////////////
	// input system.
	class	DInputManager;
	class	DInputEventQueue;
	class	DInputReceiver;
	class	DEventBase;
	class	DKeyEvent;
	class	DMouseEvent;
	class	DJoyStickEvent;
	//////////////////////////////////////////////////////////////////////////
	// DAnimation
	class	DSubSkeleton;
	typedef	SharedPtr<DSubSkeleton>			DSubSkeletonPtr;
	class	DSkeleton;
	class	DSkeletonInstance;
	typedef	SharedPtr<DSkeletonInstance>	DSkeletonInstancePtr;
	class	DBone;
	class	DAnimationController;
	class	AnimationLayer;
	class	DAnimation;
	class	DKeyFrame;
	class	DAnimationTrack;
	//////////////////////////////////////////////////////////////////////////
	// Particle
	class	DParticleSystem;
	class	DParticle;
	class	DParticleAffector;
	class	DParticleEmitter;
	//////////////////////////////////////////////////////////////////////////
	// post effect 
	class	DPostEffect;
	typedef	SharedPtr<DPostEffect>			DPostEffectPtr;
	class	DPostEffectInstance;
	typedef	SharedPtr<DPostEffectInstance>	DPostEffectInstancePtr;
	class	DPostEffectManager;
	//////////////////////////////////////////////////////////////////////////
	// scenemanager
	class	DCamera;

	class	DLightSource;

	class	DSceneInstance;
	class	DSceneManagerEnumerator;
	class	DSceneManagerFactory;
	class	DSceneManager;

	class	DRenderQueue;
	class	DRenderGroup;

	class	DMovable;
	typedef	SharedPtr<DMovable>				DMovablePtr;
	class	DEntity;
	typedef	SharedPtr<DEntity>				DEntityPtr;
	class	DSubEntity;
	typedef	SharedPtr<DSubEntity>			DSubEntityPtr;

	class	DNode;
	typedef	SharedPtr<DNode>				DNodePtr;
	class	DSceneNode;
	typedef	SharedPtr<DSceneNode>			DSceneNodePtr;

	//////////////////////////////////////////////////////////////////////////
	// terrain system
	class	DTerrain;
	class	DTerrainPage;
	typedef	SharedPtr<DTerrainPage>			DTerrainPagePtr;
	class	DTerrainManager;
	class	DTerrainQuadTreeNode;

	//////////////////////////////////////////////////////////////////////////
	// rendersystem
	class	DColor;
	class	DRenderSystem;
	class	DRenderWorkshop;
	class	DFrameBuffer;
	class	DRenderColorView;
	class	DRenderDepthView;
	class	DViewport;

	class	DShaderObject;
	typedef	SharedPtr<DShaderObject>		DShaderObjectPtr;
	class	DGpuTextureConstant;
	typedef	SharedPtr<DGpuTextureConstant>	DGpuTextureConstantPtr;
	class	DGpuProgram;
	typedef	SharedPtr<DGpuProgram>			DGpuProgramPtr;
	class	DRenderEffect;
	typedef	SharedPtr<DRenderEffect>		DRenderEffectPtr;
	class	DRenderTechnique;
	typedef	SharedPtr<DRenderTechnique>		DRenderTechniquePtr;
	class	DRenderPass;
	typedef	SharedPtr<DRenderPass>			DRenderPassPtr;

	class	DRenderWindow;
	class	DRenderable;
	class	DRenderLayout;
	typedef	SharedPtr<DRenderLayout>		DRenderLayoutPtr;
	class	DHardwareBuffer;
	typedef	SharedPtr<DHardwareBuffer>		DHardwareBufferPtr;
	class	DRawBuffer;
	typedef	SharedPtr<DRawBuffer>			DRawBufferPtr;
	class	DVertexBuffer;
	typedef	SharedPtr<DVertexBuffer>		DVertexBufferPtr;
	class	DIndexBuffer;
	typedef	SharedPtr<DIndexBuffer>			DIndexBufferPtr;
	class	DPixelBuffer;
	typedef	SharedPtr<DPixelBuffer>			DPixelBufferPtr;

	class	DVertexDeclaration;
	class	DVertexData;
	class	DVertexStreams;
	class	DIndexData;

	class	DRenderResourceFactory;

	class	DRasterizerStateObject;
	class	DDepthStencilStateObject;
	class	DBlendStateObject;
	class	DTextureSamplerStateObject;

	typedef	SharedPtr<DRasterizerStateObject>		DRasterizerStateObjectPtr;
	typedef	SharedPtr<DDepthStencilStateObject>		DDepthStencilStateObjectPtr;
	typedef	SharedPtr<DBlendStateObject>			DBlendStateObjectPtr;
//	typedef	SharedPtr<DTextureSamplerStateObject>	DTextureSamplerStateObjectPtr;
}