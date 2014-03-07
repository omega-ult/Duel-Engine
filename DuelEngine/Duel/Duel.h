/*
	[7/15/2012 OMEGA] created
	Include necessary header files, include this before using engine.

*/


#ifndef _DUEL_H_
#define _DUEL_H_

#include "DuelCommon.h"

#include "DuelAtomic.h"
#include "DuelCore.h"
#include "DuelDynLib.h"
#include "DuelDynLibManager.h"
#include "DuelPlugin.h"
#include "DuelTimer.h"

#include "DuelException.h"
#include "DuelLog.h"
#include "DuelLogManager.h"
#include "DuelBitTool.h"

#include "DuelInputEventQueue.h"
#include "DuelInputReceiver.h"
#include "DuelInputManager.h"

#include "DuelAnimation.h"
#include "DuelAnimationClip.h"
#include "DuelAnimationTrack.h"
#include "DuelBone.h"
#include "DuelKeyFrame.h"
#include "DuelTagPoint.h"

#include "DuelAutoGpuParameter.h"
#include "DuelColor.h"
#include "DuelHardwareBuffer.h"
#include "DuelRenderResourceFactory.h"
#include "DuelRenderResourceManager.h"
#include "DuelIndexBuffer.h"
#include "DuelPixelBuffer.h"
#include "DuelPixelFormat.h"
#include "DuelPostEffect.h"
#include "DuelRenderable.h"
#include "DuelRenderDeviceCaps.h"
#include "DuelRenderGroup.h"
#include "DuelRenderLayout.h"
#include "DuelRenderPass.h"
#include "DuelRenderQueue.h"
#include "DuelRenderState.h"
#include "DuelRenderSystem.h"
#include "DuelFrameBuffer.h"
#include "DuelRenderTechnique.h"
#include "DuelRenderWindow.h"
#include "DuelRenderWorkshop.h"
#include "DuelVertexBuffer.h"
#include "DuelVertexDeclaration.h"
#include "DuelViewport.h"

#include "DuelMaterial.h"
#include "DuelMaterialManager.h"

#include "DuelImageDesc.h"
#include "DuelCodec.h"
#include "DuelDDSCodec.h"
#include "DuelDMCodec.h"
#include "DuelDRECodec.h"
#include "DuelDSCodec.h"



#include "DuelArchive.h"
#include "DuelArchiveManager.h"
#include "DuelFolderArchive.h"

#include "DuelDataStream.h"
#include "DuelFileStream.h"
#include "DuelMemoryStream.h"

#include "DuelResource.h"
#include "DuelResourceManager.h"
#include "DuelResourceGroupManager.h"
#include "DuelGpuParameter.h"
#include "DuelGpuProgram.h"
#include "DuelGpuProgramManager.h"
#include "DuelMesh.h"
#include "DuelMeshManager.h"
#include "DuelRenderEffect.h"
#include "DuelRenderEffectManager.h"
#include "DuelSkeleton.h"
#include "DuelSkeletonManager.h"
#include "DuelSubMesh.h"
#include "DuelTexture.h"
#include "DuelTextureManager.h"
#include "DuelFont.h"
#include "DuelFontManager.h"
#include "DuelAudio.h"
#include "DuelAudioManager.h"

#include "DuelCamera.h"
#include "DuelLightSource.h"
#include "DuelMovable.h"
#include "DuelNode.h"
#include "DuelSceneManager.h"
#include "DuelSceneManagerEnumerator.h"
#include "DuelSceneNode.h"
#include "DuelSceneQuery.h"

#include "DuelWindowManager.h"

#include "DuelInputComponent.h"
#include "DuelInputEventQueue.h"
#include "DuelInputManager.h"
#include "DuelInputReceiver.h"
#include "DuelInputMouse.h"
#include "DuelInputKeyboard.h"
#include "DuelInputJoyStick.h"

#endif