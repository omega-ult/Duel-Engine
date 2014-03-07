//  [9/9/2012 OMEGA] created

#ifndef _DUELRENDEREFFECT_H_
#define _DUELRENDEREFFECT_H_

#include "DuelCommon.h"
#include "DuelResource.h"
#include "DuelRenderState.h"
#include "DuelGpuProgram.h"

namespace Duel
{
	// RenderEffect relies on GpuProgramManager in loading stage.
	class DUEL_API DRenderEffect : public DResource
	{
	DUEL_DECLARE_RTTI(DRenderEffect)
	public:
		friend class		DRECodec_v1_0;
		DRenderEffect(DResourceManager* fathermanager, const DString& name, const DString& groupName);
		const DString&		getName() const { return mName; }
		void				setName(const DString& name) { mName = name; }
		
		// 有很多很多technique
		typedef	std::vector<DRenderTechniquePtr>		RenderTechniques;
		typedef	VectorIterator<RenderTechniques>	RenderTechniqueIterator;
		RenderTechniqueIterator	getRenderTechniqueIterator() { return RenderTechniqueIterator(mTechniques.begin(), mTechniques.end()); }
		// get the count of the render techniques
		size_t				getTechniqueCount() const { return mTechniques.size(); }
		// add an empty, append it to the list, return the result, notice that name should be unique.
		DRenderTechniquePtr	addTechnique(const DString& name);
		// add a techinique, append it to the list
		void				addTechnique(const DRenderTechniquePtr& tech);
		// use a name to get a technique.
		DRenderTechniquePtr	getTechnique(const DString& name) ;
		// get a technique at specified index, the index should range from 0 to
		// the count of the tequnique list, otherwise a NULL will be returned.
		DRenderTechniquePtr	getTechnique(uint32 index) ;

		// remove a technique in specified index, if it existed.
		void				removeTechnique(const DString& name);
		// remove a technique in specified index, if it existed.
		void				removeTechnique(uint32 index);
		// remove all techiniques.
		void				removeAllTechniques();

		// encode this resource into a DataStream
		DDataStreamPtr		encode();
		// save this resource into a file;
		void				saveToFile(const DString& fileName);

	protected:
		// override:DResource-----------------------------------------
		void				prepareImpl() ;
		// override:DResource-----------------------------------------
		void				loadImpl() ;
		// override:DResource-----------------------------------------
		void				unloadImpl();
		// override:DResource-----------------------------------------
		size_t				calculateSize();


		// technique storage
		RenderTechniques			mTechniques;
	};



}

#endif