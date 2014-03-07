//  [12/27/2013 OMEGA] created

#ifndef _DUELTERRAIN_H_
#define _DUELTERRAIN_H_

#include "DuelCommon.h"


namespace Duel
{
	struct TerrainPageIndex
	{
		int32	x;
		int32	y;
		bool	operator == (const TerrainPageIndex& rh)
		{
			return (rh.x == x && rh.y == y);
		}
		bool	operator != (const TerrainPageIndex& rh)
		{
			return !operator==(rh);
		}
		bool	operator < (const TerrainPageIndex& rh)
		{
			if (x < rh.x)
			{
				return true;
			}
			if (x == rh.x)
			{
				return y < rh.y;
			}
			return false;
		}
	};

	class DUEL_API DTerrain : public DObject
	{
		DUEL_DECLARE_RTTI(DTerrain)
	public:
		DTerrain();
		virtual ~DTerrain();

		// interface for client to decide the action of terrain page.
		// in every update procedual, the terrain will call the handler
		// to traverse all pages.
		class DUEL_API TerrainStrategy
		{
		public:
			virtual void	handleTerrain(DTerrain* terrain) = 0;
			virtual void	handleTerrainPage(DTerrainPage* page, const DCamera* cam) = 0;
		};


		typedef	std::map<TerrainPageIndex, DTerrainPagePtr>	TerrainPageMap;
		typedef	MapIterator<TerrainPageMap>		TerrainPageIterator;
		TerrainPageIterator	getTerrainPageIterator();

		DTerrainPagePtr		getTerrainPage(TerrainPageIndex index);
		DTerrainPagePtr		createTerrainPage(TerrainPageIndex index);
		void				destroyTerrainPage(TerrainPageIndex index);

		// block size represent how many space a page could occupy,  it is not the page size,
		// but if you want watertight terrain, make sure the blockSize == pageSize*pageScale.
		void		setBlockSize(DReal size);
		DReal		getBlockSize() const;

		// call this every frame to update active pages.
		void		update(const DCamera* cam);

		// apply visible objects to the queue.
		void		applyToRenderQueue(DRenderQueue* queue, const DCamera* cam);



	protected:

		TerrainStrategy*	mStrategy;
		// this thread handles the request from pages.
		DThread				mPageWorkerThread;
		// stores the terrains.
		TerrainPageMap		mTerrainPageMap;

	};


}

#endif