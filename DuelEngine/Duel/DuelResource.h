//  [8/6/2012 OMEGA] created

#ifndef _DUELRESOURCE_H_
#define _DUELRESOURCE_H_

#include "DuelCommon.h"

namespace Duel
{

	// I referred Ogre's resource management, divide resource's loading progress into 
	// 3 stage, unload, prepare and load. 
	/*
		stage changing diagram
		     load---------------- 
		       ^         v
		       ^         v 
		 prepare-----    v
		   ^    v        v
		   ^    v        v
		unload---------------------------
	
	*/
	// because the resources in video card need to be handled in main thread, 
	// while the resources' data can be read into memory in other thread, usually
	// this IO stage is slow, so we can separate this progress into two stage,
	// prepare and load, so that we can speed-up in multi-thread environment.
	enum LoadState
	{
		LS_Unloaded,		// have not been loaded into memory
		LS_Preparing,		// usually in this stage, the data is being loaded into memory.
		LS_Prepared,		// usually in this stage, the data has been loaded into memory. We can use it to create video card resource.
		LS_Loading,			// usually in this stage, the data will be used in resource creation.
		LS_Loaded,			// loaded in memory
		LS_Unloading,		// unloading from memory
	};

	// all resource must inherite this class
	class DUEL_API DResource : public DObject
	{
	DUEL_DECLARE_RTTI(DResource)
	public:
		DResource(DResourceManager* fathermanager, const DString& name, const DString& groupName, const DString& type);
		virtual ~DResource();

		DUEL_AUTO_MUTEX			// public allow external lock
		//////////////////////////////////////////////////////////////////////////
		// signals block
		// call back function used when preparing complete
		DSignal<void(DResource* r)>			
			signalPreparingingCompleted;
		// call back function used when loading complete
		DSignal<void(DResource* r)>			
			signalLoadingCompleted;
		// called when a resource is trying to change its group.
		DSignal<void(DResource* newRes, const DString& oldGroup)>	
			signalGroupChanged;
		// call back function used when unloading complete
		DSignal<void(DResource* r)>			
			signalUnloadCompleted;
		// if someone touch this resource, the listeners will
		// be notified, it is useful in resource pool management.
		DSignal<void(DResource* r)>			
			signalBeTouched;
		//////////////////////////////////////////////////////////////////////////

		// get a pointer to its creator
		DResourceManager*	getManager() { return mFatherManager; }
		// get the resource's name
		const DString&		getName() { return mName; }
		// get the resource's groupName
		const DString&		getGroupName() { return mGroupName; }
		// change group name to join a new group.
		void				changeGroup(const DString& newGroup);

		// optional for loading this resource, 
		// set the source data which is used in loading. notice that
		// sub-class may not use this data if you set a new data after
		// the resource has been prepared.
		virtual	void		setSourceData(const DDataStreamPtr& data);
		virtual DDataStreamPtr	getSourceData() const;

		// optional for loading this resource
		// set the codec for codec/decode this resourece.
		virtual void		setCodec(DResourceCodec* codec) { mCodec = codec; }
		// get the codec of this resource.
		virtual DResourceCodec*	getCodec() const { return mCodec; }


		// load a resource into memory, usually do IO here. call load() will 
		// automatically call it.
		virtual	void		prepare();

		// create hardware-related resource, if this method is executed
		// in mutithread environment, it will do thread synchronization
		virtual	void		load();
		// reload, just unload(); and load();
		virtual	void		reload();
		// free memory that its holds.
		virtual void		unload();

		// touch this resource means someone is trying to use it, 
		// useful in resource pool management.
		virtual	void		touch();
		// query the loading state.
		LoadState			getLoadState() { return mLoadState.get();}
		// query whether the resource is loaded. i.e things are in memory already.
		bool				isLoaded() { return (mLoadState.get() == LS_Loaded); }
		// query whether the resource is loading.
		bool				isLoading() { return (mLoadState.get() == LS_Loading); }

		// get the size of the resource.
		size_t				getSize() { return mSize; }

	protected:
		// sub-class just finish these methods, for conciseness, other thread operation
		// is done in load() function, main loading logic is done here.
		// return true if this resource has been loaded successfully.
		virtual void		loadImpl()  = 0;
		// sub-class just finish these methods, for conciseness, other thread operation
		// is done in unload() function, main loading logic is done here.
		virtual	void		unloadImpl() = 0;
		// sub-class must implement this method, which is used to calculate memory usage.
		virtual	size_t		calculateSize() = 0;

		// Internal implementation of the meat of the 'prepare' action.  
		// return true if this resource has been successfully prepared.
		// this method will stores the data into mFreshData, which can be used directly
		// in the loadImpl();
		virtual void		prepareImpl() ;
		// Internal implementation of the meat of the 'unprepare' action.  
		virtual	void		unprepareImpl();

		// pointer to the manager who create it.
		DResourceManager*	mFatherManager;
		// name of the resource, MUST be unique
		DString				mName;
		// the name of group of which the resource should be in
		DString				mGroupName;
		// type of this resource.
		DString				mType;
		// the memory size the resource take
		size_t				mSize;
		// flag indicate loading state.
		DAtomicScalar<LoadState>	mLoadState;

		// the data stream that contains the resource.
		DDataStreamPtr		mExternSrc;
		// codec used for codec/decode this resource.
		DResourceCodec*		mCodec;
		// store data in the memory. which will stores the prepared data, 
		// loadImpl can reach this data directly because the prepare procedure will
		// make sure the freshdata is available.
		DMemoryStreamPtr	mPreparedData;

	private:
		// not allowed
		DResource() {}
	};


}



#endif