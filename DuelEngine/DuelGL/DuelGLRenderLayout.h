//  [8/28/2013 OMEGA] created

#ifndef _DUELGLRENDERLAYOUT_H_
#define _DUELGLRENDERLAYOUT_H_

#include "DuelRenderLayout.h"
#include "DuelGLCommon.h"

namespace Duel
{

	class DUELGL_API GLRenderLayout : public DRenderLayout
	{
	DUEL_DECLARE_RTTI(GLRenderLayout)
	public:
		GLRenderLayout(DRenderResourceFactory* creator);
		virtual ~GLRenderLayout();
		// override : DRenderLayout--------------
		virtual	void	seal();

		GLuint			getVAO() 
		{ 
			if (mVAO == NULL)
			{
				seal();
			}
			return mVAO; 
		}

	protected:
		GLuint			mVAO;
	};

}

#endif