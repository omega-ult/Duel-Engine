//  [4/7/2013 OMEGA] created

#include "Duel.h"
#include "DuelGLCommon.h"

namespace Duel
{

	class DUELGL_API GLPlugin : public DPlugin
	{
	public:
		GLPlugin();
		~GLPlugin();
		virtual const DString& getName() const
		{
			return mName;
		}

		virtual void install();

		virtual void initialize();

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