//  [9/19/2012 OMEGA] created

#ifndef _DUELGPUPROGRAMMANAGER_H_
#define _DUELGPUPROGRAMMANAGER_H_

#include "DuelCommon.h"
#include "DuelResourceManager.h"
#include "DuelGpuProgram.h"

namespace Duel
{
	// Parameter that used to create GpuProgram,
	class DUEL_API DGpuProgramDescription : public DResourceDescription
	{
	DUEL_DECLARE_RTTI(DGpuProgramDescription)
	public:
		DGpuProgramDescription(const DString& name, const DString& groupName);
		// entry function name, must be indentical to the GpuProgram
		DString	entry;
		// profile of the program, it can be "vs_2_0" , "ps_2_0" , "GL_VERTEX_SHADER" or "GL_FRAGMENT_SHADER", etc.
		DString	profile;
		// the language in which the program is written. possible value is 
		// "cg" "hlsl" "glsl", default value is blank("")
		DString	language;
		// indicate whether this program is complied.
		bool	compiled;
	};

	// this class defines some common interfaces for sub-class, each rendersystem
	// should have its own GpuProgramManager.
	class DUEL_API DGpuProgramManager : public DResourceManager, public DSingleton<DGpuProgramManager>
	{
	DUEL_DECLARE_RTTI(DGpuProgramManager)
	public:
		DGpuProgramManager();
		~DGpuProgramManager();
		/** Override standard DSingleton retrieval.
        @remarks
        Why do we do this? Well, it's because the DSingleton
        implementation is in a .h file, which means it gets compiled
        into anybody who includes it. This is needed for the
        DSingleton template to work, but we actually only want it
        compiled into the implementation of the class based on the
        DSingleton, not all of them. If we don't change this, we get
        link errors when trying to use the DSingleton-based class from
        an outside dll.
        @par
        This method just delegates to the template version anyway,
        but the implementation stays in this single compilation unit,
        preventing link errors.
        */
// 		static GpuProgramManager& getSingleton();
// 		static GpuProgramManager* getSingletonPtr();

		// override:DResourceManager-----------------------------------------------
		// data content should be an XML, format like:
		// <GpuResourceParameter>
		//		<Name>xxxx</Name>
		//		<Group>xxxx</Group>
		//		<FileName>xxxx</FileName> <!-- this value can be empty -->
		//		<MatrixPacking>row_major</MatrixPacking> <!-- one of "row_major" and "column_major" -->
		//		<Entry>main</Entry>
		//		<Profile>vs_2_0</Profile>	
		//		<Language>hlsl</Language>
		//		<Compiled>true</Compiled>
		// </GpuResourceParameter>
		// <GpuResourceParameter>
		//		............
		//		............
		// </GpuResourceParameter>
		ResourceDescriptionList	parseParameter(DDataStream* data);
	protected:
		// because different program have different property, make this manager do all of the 
		// work is impossible, so leave it to sub-class.
		//virtual GpuProgram*	createImpl(DResourceParameter* createParam) = 0;
	};
}

#endif


