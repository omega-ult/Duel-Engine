//  [10/29/2012 OMEGA] created

#ifndef _DUELMESHMANAGER_H_
#define _DUELMESHMANAGER_H_

#include "DuelCommon.h"
#include "DuelResourceManager.h"
#include "DuelMesh.h"

namespace Duel
{
	
	class DUEL_API DMeshDescription : public DResourceDescription
	{
	DUEL_DECLARE_RTTI(DMeshDescription)
	public:
		DMeshDescription(const DString& name, const DString& groupName);

		// format that used to identify the codec for loading. 
		// now only "dm" supported.
		DString	fileFormat;
		// indicate whether to use shadow buffer, which trade memory space for reading/writing speed
		bool	shadowBuffer;

	};

#define DEFAULT_MESH_CODEC	DMCodec_v1_0

	class DUEL_API DMeshManager : public DResourceManager
	{
	DUEL_DECLARE_RTTI(DMeshManager)
	public:
		// default constructor will set the default DMeshCodec 
		DMeshManager();

		// override:DResourceManager--------------------------------
		// data content should be an XML, format like:
		// <MeshResourceParameter>
		//		<Name>xxxx</Name>
		//		<Group>xxxx</Group>
		//		<FileFormat>xxxx</FileFormat> 
		//		<ShadowBuffer>true</ShadowBuffer> 
		// </MeshResourceParameter>
		// <MeshResourceParameter>
		//		............
		//		............
		// </MeshResourceParameter>

		ResourceDescriptionList parseParameter(DDataStream* data);
	protected:
		// override:DResourceManager--------------------------------
		DResource*	createImpl(DResourceDescription* createParam);
		
		// common codec used by newly created mesh
		// current: DMCodec_v1_0
		DResourceCodec*	mCommonCodec;
	};

}

#endif