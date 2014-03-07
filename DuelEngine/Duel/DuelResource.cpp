//  [8/6/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelException.h"
#include "DuelResource.h"
#include "DuelResourceGroupManager.h"
#include "DuelMemoryStream.h"
#include "DuelLogManager.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DResource, DObject);
	
	DResource::DResource( DResourceManager* fathermanager,
						const DString& name, const DString& groupName, const DString& type ) :
		fatherManager(fathermanager), mName(name), mGroupName(groupName), mType(type),
		mLoadState(LS_Unloaded)
	{

	}
	
	DResource::~DResource()
	{
		unload();
	}

	void DResource::changeGroup( const DString& newGroup )
	{
		DString	oldGroupName = mGroupName;
		mGroupName = newGroup;
		// emit signals.
		if (!signalGroupChanged.empty())
		{
			signalGroupChanged(this, oldGroupName);	
		}	
	}

	// TODO: 
	// 并发测试
	void DResource::prepare()
	{
		// quick check that avoids any synchronisation
		LoadState old = mLoadState.get();
		if (old != LS_Unloaded && old != LS_Preparing) return;

		// atomically do slower check to make absolutely sure,
		// and set the load state to PREPARING
		if (!mLoadState.cas(LS_Unloaded, LS_Preparing))
		{
			while( mLoadState.get() == LS_Preparing )
			{
				DUEL_LOCK_AUTO_MUTEX
			}

			LoadState state = mLoadState.get();
			if( state != LS_Prepared && state != LS_Loading && state != LS_Loaded )
			{
// 				DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters, 
// 					"Another thread failed in resource operation",
// 					"Duel::Resource::prepare");
			}
			return;
		}

		// Scope lock for actual loading
// 		try
// 		{

		DUEL_LOCK_AUTO_MUTEX
		try
		{
			prepareImpl();
			signalPreparingingCompleted(this);
			mLoadState.set(LS_Prepared);
		}
		catch (DException* e)
		{
			DLogManager::getSingleton().logMessage("DLog", "Failed to prepare resource : " + mGroupName + "/" + mName
				+ ", in source : " + e->getFile() + ", line : " + DStringTool::toString(e->getLine())
				+ ", with : " + e->getFullDescription(), DLog::LL_Warn);
#ifdef DUEL_DEBUG
			std::cout<< "Failed to prepare resource : " + mGroupName + "/" + mName + "\n";
			std::cout<< e->getSource() << "\n" << e->getDescription();
#endif
			mLoadState.set(LS_Unloaded);
		}
// 		if(prepareImpl())
// 		{
// 			mLoadState.set(LS_Prepared);
// 			signalPreparingingCompleted(this);
// 		}
// 		else
// 		{
// 			mLoadState.set(LS_Unloaded);
// 		}


	}

	// TODO: 
	// 并发测试
	void DResource::load()
	{
		if (mLoadState.get() != LS_Prepared)
		{
			prepare();
		}
		// Early-out without lock (mitigate perf cost of ensuring loaded)
		// Don't load if:
		// 1. We're already loaded
		// 2. Another thread is loading right now
		// 3. We're marked for background loading and this is not the background
		//    loading thread we're being called by

		//if (mIsBackgroundLoaded && !background) return;

		// This next section is to deal with cases where 2 threads are fighting over
		// who gets to prepare / load - this will only usually happen if loading is escalated
		bool keepChecking = true;
		LoadState old = LS_Unloaded;
		while (keepChecking)
		{
			// quick check that avoids any synchronisation
			old = mLoadState.get();

			if ( old == LS_Preparing )
			{
				while( mLoadState.get() == LS_Preparing )
				{
					DUEL_LOCK_AUTO_MUTEX
				}
				old = mLoadState.get();
			}

			if (old != LS_Unloaded && old != LS_Prepared && old != LS_Loading) return;

			// atomically do slower check to make absolutely sure,
			// and set the load state to LOADING
			if (old==LS_Loading || !mLoadState.cas(old, LS_Loading))
			{
				while( mLoadState.get() == LS_Loading )
				{
					DUEL_LOCK_AUTO_MUTEX
				}

				LoadState state = mLoadState.get();
				if( state == LS_Prepared || state == LS_Preparing )
				{
					// another thread is preparing, loop around
					continue;
				}
				else if( state != LS_Loaded )
				{

// 					DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
// 						"Another thread failed in resource operation",
// 						"Duel::Resource::load");
				}
				return;
			}
			keepChecking = false;
		}

// 		// Scope lock for actual loading
// 		try
// 		{

		DUEL_LOCK_AUTO_MUTEX
		try
		{
			loadImpl();
			mLoadState.set(LS_Loaded);
			signalLoadingCompleted(this);
			// Calculate resource size
			mSize = calculateSize();
		}
		catch (DException* e)
		{
			DLogManager::getSingleton().logMessage("DLog", "Failed to load resource : " + mGroupName + "/" + mName
				+ ", in source : " + e->getFile() + ", line : " + DStringTool::toString(e->getLine())
				+ ", with : " + e->getFullDescription(), DLog::LL_Warn);
#ifdef DUEL_DEBUG
			std::cout<< "Failed to load resource : " + mGroupName + "/" + mName + "\n";
			std::cout<< e->getSource() << "\n" << e->getDescription();
#endif
			mLoadState.set(old);
		}
// 
// 		if (loadImpl())
// 		{
// 			mLoadState.set(LS_Loaded);
// 			signalLoadingCompleted(this);
// 			// Calculate resource size
// 			mSize = calculateSize();
// 		}
// 		else // failed to load.
// 		{
// 			DLogManager::getSingleton().logMessage("Failed to load " + mType +" resource: " + mGroupName + "/" + mName);
// 			mLoadState.set(LS_Prepared); // set unloaded to notify other thread.
// 		}
// 		}
// 		catch (...)
// 		{
// 			// Reset loading in-progress flag, in case failed for some reason.
// 			// We reset it to UNLOADED because the only other case is when
// 			// old == PREPARED in which case the loadImpl should wipe out
// 			// any prepared data since it might be invalid.
// 			mLoadState.set(LS_Unloaded);
// 			// Re-throw
// 			throw;
// 		}


	}

	void DResource::reload()
	{
		DUEL_LOCK_AUTO_MUTEX
		if (mLoadState.get() == LS_Loaded)
		{
			unload();
			load();
		}
	}

	void DResource::unload()
	{
		// Early-out without lock (mitigate perf cost of ensuring unloaded)
		LoadState old = mLoadState.get();
		if (old!=LS_Loaded && old!=LS_Prepared) return;


		if (!mLoadState.cas(old,LS_Unloading)) return;

		// Scope lock for actual unload
		{
			DUEL_LOCK_AUTO_MUTEX;
			if (old==LS_Prepared)
			{
				unprepareImpl();
			} 
			else 
			{
				unloadImpl();
			}
		}

		mLoadState.set(LS_Unloaded);

		signalUnloadCompleted(this);
		// Early-out without lock (mitigate perf cost of ensuring unloaded)
// 		LoadState oldState = mLoadState;
// 		if (oldState != LS_LOADED)
// 		{
// 			return;
// 		}
// 		if(!mLoadState.cas(oldState, LS_UNLOADING))
// 		{
// 			// another thread is doing unloading.
// 			return;
// 		}
// 		// scope lock for actual unload
// 		{
// 			DUEL_LOCK_AUTO_MUTEX
// 			unloadImpl();
// 		}
// 		mLoadState.set(LS_UNLOADED);
// 
// 		// notify listeners
// 		if (oldState == LS_LOADED)
// 		{
// 			// emit signals.
// 			if (!signalUnloadCompleted.empty())
// 			{
// 				signalUnloadCompleted(this);
// 			}
// 		}
	}


	void DResource::touch()
	{
		// make sure resource is loaded.
		load();
		// emit signals.
		signalBeTouched(this);
	}

	void DResource::setSourceData( const DDataStreamPtr& data )
	{
		DUEL_LOCK_AUTO_MUTEX
		mExternSrc = data;
	}

	Duel::DDataStreamPtr DResource::getSourceData() const
	{
		return mExternSrc;
	}

	void DResource::prepareImpl() 
	{
		DDataStreamPtr data;
		if (mExternSrc != NULL)
		{
			data = mExternSrc;
		}
		else
		{
			data  = DResourceGroupManager::getSingleton().openResource(this);
			if (data == NULL)
			{
				DUEL_EXCEPT_BRIEF(DException::ET_ItemNotFound,
					"Specified resource does not exist in the group : " + mGroupName + "/" + mName,
					"Duel::DResource::prepareImpl")
			}
		}
		if (data->isValid())
		{
			DMemoryStream* memData = new DMemoryStream(data.get());
			data->read(memData->getChunkPtr(), data->getSize());
			mPreparedData.reset(memData);
		}
		else
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Specified data is invalid : " + data->getName(),
				"Duel::DResource::prepareImpl")

		}
	}

	void DResource::unprepareImpl()
	{
		mExternSrc.reset();
		mPreparedData.reset();
	}

}