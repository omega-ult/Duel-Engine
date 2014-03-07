//  [8/10/2012 OMEGA] created

#ifndef _DUELHARDWAREBUFFER_H_
#define _DUELHARDWAREBUFFER_H_

#include "DuelCommon.h"

namespace Duel
{
	enum HardwareBufferUsage
	{
        /** Static buffer which the application rarely modifies once created. Modifying 
        the contents of this buffer will involve a performance hit.
        */
        HBU_Static = 0x01,
		/** Indicates the application would like to modify this buffer with the CPU
		fairly often. 
		Buffers created with this flag will typically end up in AGP memory rather 
		than video memory.
		*/
		HBU_Dynamic = 0x02,
		/** Indicates the application will never read the contents of the buffer back, 
		it will only ever write data. Locking a buffer with this flag will ALWAYS 
		return a pointer to new, blank memory rather than the memory associated 
		with the contents of the buffer; this avoids DMA stalls because you can 
		write to a new memory area while the previous one is being used. 
		*/
		HBU_WriteOnly = 0x04,
        /** Indicates that the application will be refilling the contents
        of the buffer regularly (not just updating, but generating the
        contents from scratch), and therefore does not mind if the contents 
        of the buffer are lost somehow and need to be recreated. This
        allows and additional level of optimisation on the buffer.
        This option only really makes sense when combined with 
        HBU_DYNAMIC_WRITE_ONLY.
        */
        HBU_Discardable = 0x08,
		/// Combination of HBU_STATIC and HBU_WRITE_ONLY
		HBU_StaticWriteOnly = 0x05, 
		/** Combination of HBU_DYNAMIC and HBU_WRITE_ONLY. If you use 
        this, strongly consider using HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE
        instead if you update the entire contents of the buffer very 
        regularly. 
        */
		HBU_DynamicWriteOnly = 0x06,
        /// Combination of HBU_DYNAMIC, HBU_WRITE_ONLY and HBU_DISCARDABLE
        HBU_DynamicWriteOnlyDiscardable = 0x0e,
	};

	/// Locking options
	enum HardwareBufferLock
	{
        /** Normal mode, ie allows read/write and contents are preserved. */
        HBL_Normal,
		/** Discards the <em>entire</em> buffer while locking; this allows optimisation to be 
		performed because synchronisation issues are relaxed. Only allowed on buffers 
		created with the HBU_DYNAMIC flag. 
		*/
		HBL_Discard,
		/** Lock the buffer for reading only. Not allowed in buffers which are created with HBU_WRITE_ONLY. 
		Mandatory on static buffers, i.e. those created without the HBU_DYNAMIC flag. 
		*/ 
		HBL_ReadOnly,
        /** As HBL_NORMAL, except the application guarantees not to override any 
        region of the buffer which has already been used in this frame, can allow
        some optimisation on some APIs. */
        HBL_NoOverwrite			
	};

	/** Abstract class defining common features of hardware buffers.
    @remarks
 	    A 'hardware buffer' is any area of memory held outside of core system ram,
	    and in our case refers mostly to video ram, although in theory this class
	    could be used with other memory areas such as sound card memory, custom
	    coprocessor memory etc.
    @par
 	    This reflects the fact that memory held outside of main system RAM must 
	    be interacted with in a more formal fashion in order to promote
	    cooperative and optimal usage of the buffers between the various 
	    processing units which manipulate them.
    @par
 	    This abstract class defines the core interface which is common to all
	    buffers, whether it be vertex buffers, index buffers, texture memory
	    or framebuffer memory etc.
	@par
		Buffers have the ability to be 'shadowed' in system memory, this is because
		the kinds of access allowed on hardware buffers is not always as flexible as
		that allowed for areas of system memory - for example it is often either 
		impossible, or extremely undesirable from a performance standpoint to read from
		a hardware buffer; when writing to hardware buffers, you should also write every
		byte and do it sequentially. In situations where this is too restrictive, 
		it is possible to create a hardware, write-only buffer (the most efficient kind) 
		and to back it with a system memory 'shadow' copy which can be read and updated arbitrarily.
		
    */
	class DUEL_API DHardwareBuffer : public DObject
	{
	DUEL_DECLARE_RTTI(DHardwareBuffer)
	public:
		// don't use default construtctor!
		// subclass must call the only type of the hardware buffer:
		// DHardwareBuffer(HardwareBufferUsage usage, size_t size);
		//DHardwareBuffer();
		virtual ~DHardwareBuffer();

		// the properties of a hardware buffer are unchangenable
		DHardwareBuffer(HardwareBufferUsage usage);

		// Lock the buffer for (potentially) reading / writing.
		virtual	void*	lock(size_t offset, size_t size, HardwareBufferLock lockType);
		// lock the entire buffer
		virtual void*	lock(HardwareBufferLock lockType);

		// Releases the lock on this buffer. 
		virtual	void	unlock();

		/** Reads data from the buffer and places it in the memory pointed to by pDest.
		@param offset The byte offset from the start of the buffer to read
		@param length The size of the area to read, in bytes
        @param pDest The area of memory in which to place the data, must be large enough to 
            accommodate the data!
        */
        virtual void	readData(size_t offset, size_t length, void* pDest) = 0;
        /** Writes data to the buffer from an area of system memory; note that you must
            ensure that your buffer is big enough.
		@param offset The byte offset from the start of the buffer to start writing
		@param length The size of the data to write to, in bytes
        @param pSource The source of the data to be written
		@param discardWholeBuffer If true, this allows the driver to discard the entire buffer when writing,
			such that DMA stalls can be avoided; use if you can.
        */
        virtual void	writeData(size_t offset, size_t length, const void* pSource,
				bool discardWholeBuffer = false) = 0;



		// get the size in byte of the buffer
		virtual	size_t	getSize();
		// get the usage of this DHardwareBuffer
		HardwareBufferUsage	getUsage();
		// query whether this buffer is locked.
		bool				isLocked();
	protected:
		// sub-class should implement these:
		virtual void*		lockImpl(size_t offset, size_t size, HardwareBufferLock locklockType) = 0;
		virtual void		unlockImpl() = 0;

		size_t				mByteSize;
		size_t				mLockStart;
		size_t				mLockSize;
		HardwareBufferUsage	mUsage;
		HardwareBufferLock	mLockType;
		bool				mbIsLocked;

	private:
		// not allowed
		DHardwareBuffer();
	};

	// raw buffer located in the main memory,
	// it is always HBU_Dynamic and can be locked use HBL_Normal
	class DUEL_API DRawBuffer : public DHardwareBuffer
	{
		DUEL_DECLARE_RTTI(DRawBuffer)
	public:
		DRawBuffer(size_t byteSize);
		// create a copy of specified src buffer.
		DRawBuffer(DHardwareBuffer* src);
		~DRawBuffer();

		virtual void readData( size_t offset, size_t length, void* pDest );

		virtual void writeData( size_t offset, size_t length, const void* pSource, bool discardWholeBuffer = false );

	protected:
		virtual void* lockImpl( size_t offset, size_t size, HardwareBufferLock locklockType );

		virtual void unlockImpl();

		uint8*		mData;
	};
}


#endif