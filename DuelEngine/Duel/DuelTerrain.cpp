//  [12/27/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelTerrain.h"
#include "DuelTerrainPage.h"
#include "DuelCamera.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DTerrain, DObject);


	DTerrain::DTerrain()
	{

	}

	DTerrain::~DTerrain()
	{

	}

	Duel::DTerrainPagePtr DTerrain::getTerrainPage( TerrainPageIndex index )
	{
		return DTerrainPagePtr();
	}

	DTerrain::TerrainPageIterator DTerrain::getTerrainPageIterator()
	{
		return TerrainPageIterator(mTerrainPageMap);
	}

	void DTerrain::applyToRenderQueue( DRenderQueue* queue, const DCamera* cam )
	{
		// clip terrain pages.
	}

	void DTerrain::update(const DCamera* cam)
	{

		// update terrain pages.
// 		TerrainPageIterator itor = getTerrainPageIterator();
// 		while (itor.hasMoreElements())
// 		{
// 			DTerrainPagePtr page = itor.getNext();
// 
// 			if (cam->isInside(page->getBoundingBox()))
// 			{
// 
// 			}
// 		}
	}

}