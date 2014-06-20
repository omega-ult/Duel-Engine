//  [11/8/2012 OMEGA] created

#ifndef _DUELVERTEXDATA_H_
#define _DUELVERTEXDATA_H_

#include "DuelCommon.h"
#include "DuelVertexBuffer.h"
#include "DuelVertexDeclaration.h"


namespace Duel
{
	/** Records the state of all the vertex buffer bindings required to provide a vertex declaration
		with the input data it needs for the vertex elements.
	@remarks
		Why do we have this binding list rather than just have VertexElement referring to the
		vertex buffers direct? Well, in the underlying APIs, binding the vertex buffers to an
		index (or 'stream') is the way that vertex data is linked, so this structure better
		reflects the realities of that. In addition, by separating the vertex declaration from
		the list of vertex buffer bindings, it becomes possible to reuse bindings between declarations
		and vice versa, giving opportunities to reduce the state changes required to perform rendering.
	@par
		Like the other classes in this functional area, these binding maps should be created and
		destroyed using the HardwareBufferManager.
	*/
	class DUEL_API DVertexStreams : public DObject
	{
	DUEL_DECLARE_RTTI(DVertexStreams)
	public:
		DVertexStreams();
		virtual ~DVertexStreams();
		// the key(uint16) is the source in the DVertexDeclaration.
		typedef std::map<uint16, DVertexBufferPtr>	VertexStreamMap;
		typedef	MapIterator<VertexStreamMap>		VertexStreamIterator;
		VertexStreamIterator	getVertexStreamIterator() { return VertexStreamIterator(mStreamMap.begin(), mStreamMap.end()); }
		/** Set a binding, associating a vertex buffer with a given index.
		@remarks
			If the index is already associated with a vertex buffer,
            the association will be replaced. This may cause the old buffer
            to be destroyed if nothing else is referring to it.
			You should assign bindings from 0 and not leave gaps, although you can
			bind them in any order.
		*/
		virtual	void			setStream(uint16 source, const DVertexBufferPtr& vertices);
		// if specified source does not exist, return NULL.
		virtual	DVertexBufferPtr	getStream(uint16 source);
		/** Removes an existing binding. */
		virtual	void			clearStream(uint16 source);
		// remove all bindings.
		virtual	void			clearAllStreams();
		
		virtual	size_t			getStreamCount() const { return mStreamMap.size(); }

	protected:
		VertexStreamMap		mStreamMap;
	};



	// a wrap of vertex buffer and its declaration, not guarantee for 
	// valid pointer.
	class DUEL_API DVertexData : public DObject
	{
	DUEL_DECLARE_RTTI(DVertexData)
	public:
		DVertexData() {}
		DVertexData(const DVertexStreams& streams, const DVertexDeclaration& decl) : mStreams(streams),mDeclaration(decl){}
		
		void	setStreams(const DVertexStreams& vs) { mStreams = vs; }
		void	setDeclaration(const DVertexDeclaration& vd) { mDeclaration = vd; }
		const DVertexStreams&		getBufferStreams() const { return mStreams; }
		const DVertexDeclaration&	getDeclaration() const { return mDeclaration; }

	protected:
		DVertexStreams		mStreams;
		DVertexDeclaration	mDeclaration;
	};


}


#endif