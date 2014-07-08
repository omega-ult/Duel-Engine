//  [4/7/2013 OMEGA] created
#ifndef _DUELGLPLUGIN_H_
#define _DUELGLPLUGIN_H_

#include "Duel.h"
#include "DuelGLCommon.h"

namespace Duel
{

	class DUELGL_API GLPlugin : public DPlugin
	{
	DUEL_DECLARE_RTTI(GLPlugin)
	public:
		GLPlugin();
		~GLPlugin();
		virtual const DString& getName() const
		{
			return mName;
		}

		virtual void install();

		virtual void initialize( const DString& config );

		virtual void shutdown();

		virtual void uninstall();
	protected:
		DString			mName;
		GLRenderSystem*	mRenderSystem;
		GLRenderResourceFactory*	mRenderResFactory;
		GLTextureManager*	mTextureManager;
		GLGpuProgramManger*	mGpuProgramManager;
		DRenderResourceManager* mhbManager;

	};
	GLPlugin*	glPlugin;
}
#endif
