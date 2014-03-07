//  [9/2/2012 OMEGA] created

#ifndef _DUELGPUPROGRAM_H_
#define _DUELGPUPROGRAM_H_

#include "DuelCommon.h"
#include "DuelResource.h"
#include "DuelGpuParameter.h"

namespace Duel
{
	enum GpuProgramType
	{
		GPT_Vertex,
		GPT_Geometry,	// not support yet
		GPT_Pixel
	};

	// NOTICE:
	// a gpu program may have undefined gpuparameters if it was not been build
	// through DShaderObject::build, (especially in OpenGL, because its uniforms
	// are retrieved after the program have been linked)
	class DUEL_API DGpuProgram : public DResource
	{
	DUEL_DECLARE_RTTI(DGpuProgram)
	public:
		DGpuProgram(DResourceManager* fathermanager, const DString& name, const DString& groupName);
		virtual ~DGpuProgram();
		// set the program type, it is neccessary when compiling happened.
		void				setProgramType(GpuProgramType type);
		GpuProgramType		getProgramType();
		// Did this program encounter a compile error?
		virtual bool			hasCompileError(void) const;

		// Reset a compile error if it occurred, allowing the load to be retried
		virtual void			resetCompileError(void);

		// get the language of the DGpuProgram, for example hlsl cg.
		virtual	const DString&	getLanguage(void) const = 0;

		// use this function to compile shader.
		const DString&	getSourceCode() { return mSourceCode; }

	protected:
		// indicate what type is this program.
		GpuProgramType	mProgramType;

		// override:DResource----------------------
		void			loadImpl() ;
		// override:DResource----------------------
		virtual void	unloadImpl();

		// sub-class can only load from source.
		virtual	void	loadFromSource() = 0;
		
		// if this flag is true. program data will be got from archive,
		// otherwise, program is located in mSource.
		bool			mbUseFile;
		// the string code of file, can be used for compile.
		DString			mSourceCode;

		// if we failed to compile, this flag should be true.
		bool			mbFailed;
	};




}

#endif