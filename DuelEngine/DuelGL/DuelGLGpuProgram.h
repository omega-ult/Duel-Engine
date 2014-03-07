//  [4/7/2013 OMEGA] created

#ifndef _DUELGLGPUPROGRAM_H_
#define _DUELGLGPUPROGRAM_H_

#include "Duel.h"
#include "DuelGLCommon.h"

namespace Duel
{
	class DUELGL_API GLGpuProgram : public DGpuProgram
	{
	DUEL_DECLARE_RTTI(GLGpuProgram)
	public:
		GLGpuProgram(DResourceManager* fathermanager, const DString& name, const DString& groupName);
		
		// override : DGpuProgram
		virtual const DString& getLanguage( void ) const;


		void			setProfile(const DString& profile);
		const DString&	getProfile() const;

		void			setEntry(const DString& entry);
		const DString&	getEntry() const;

		const DString&	getCompilationError();

		// public for GLShaderObject
		INTERNAL void	setCompileErrorFlag(bool flag) { mbFailed = flag; }
		INTERNAL void	setCompileError(const DString& err) { mCompileError = err; }
	protected:		
		// because the OpenGL's parameters is constructed in linking operation, so
		// we do nothing in the buildParameters, instead we leave this work
		// to the GLShaderObject::build.
		// override : DGpuProgram
		virtual void	loadFromSource();
		// override : DGpuProgram
		virtual size_t	calculateSize();
		static DString		msLanguage;
		DString				mProfile;
		DString				mEntry;

		DString				mCompileError;

	};

}

#endif