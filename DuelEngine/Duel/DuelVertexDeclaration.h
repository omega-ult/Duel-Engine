//  [8/10/2012 OMEGA] created

#ifndef _DUELVERTEXDECLARATION_H_
#define _DUELVERTEXDECLARATION_H_

#include "DuelCommon.h"
#include "DuelVertexBuffer.h"

namespace Duel
{
	// Vertex element semantics, used to identify 
	// the meaning of vertex buffer contents
	enum VertexElementSemantic
	{
		// position, using 3 Reals per vertex
		VES_Position		= 0,
		// blending weight
		VES_BlendWeight		= 1,
		// blending indices
		VES_BlendIndices	= 2,
		// normal, using 3 Reals per vertex
		VES_Normal			= 3,
		// diffuse color
		VES_Diffuse			= 4,
		// specular color
		VES_Specular		= 5,
		// texture coordinates, support up to 7 texcoords(with index in DVertexElement)
		VES_TexCoord		= 6,
		// binormal(Y axis if normal is Z)
		VES_Binormal		= 14,
		// tangent (X axis if normal is Z)
		VES_Tangent			= 15
	};

	// Vertex element type, used to identify the base types of the vertex contents
	// notice that there are more available types in specified render system, but
	// these types are enough for us.
	enum VertexElementType
	{
		/// D3D style compact colour	// byte for each chanel.
		VET_ARGB	= 0,
		/// GL style compact colour
		VET_ABGR	= 1,
		VET_Byte4	= 2,
		VET_UByte4	= 3,
		VET_Float1	= 4,
		VET_Float2	= 5,
		VET_Float3	= 6,
		VET_Float4	= 7,
		VET_Short1	= 8,
		VET_Short2	= 9,
		VET_Short3	= 10,
		VET_Short4	= 11,
		VET_UShort1	= 12,
		VET_UShort2	= 13,
		VET_UShort3	= 14,
		VET_UShort4	= 15,
		VET_Int1	= 16,
		VET_Int2	= 17,
		VET_Int3	= 18,
		VET_Int4	= 19,
		VET_UInt1	= 20,
		VET_UInt2	= 21,
		VET_UInt3	= 22,
		VET_UInt4	= 23
	};

	// since OpenGL's support for Instance Drawing is very limited, so we do not recommend
	// you to use that way, here the source could just set to 0 and the bufType and instanceStepRate
	// will be ignored in OpenGL for most case.
	class DUEL_API DVertexElement : public DObject
	{
	DUEL_DECLARE_RTTI(DVertexElement)
	public:
		DVertexElement(){}
		virtual ~DVertexElement(){}

		/// Constructor, should not be called directly, instead, call DVertexDeclaration::addElement
		DVertexElement(uint16 source, size_t offset, VertexElementType type,
			VertexElementSemantic semantic, uint16 index = 0, 
			VertexBufferType bufType = VBT_Geometry, uint32 instanceStepRate = 0);

		/// Gets the vertex buffer index from where this element draws it's values
		uint16			getSource(void) const { return mSource; }
		/// Gets the offset into the buffer where this element starts
		size_t			getOffset(void) const { return mOffset; }
		/// Gets the data format of this element
		VertexElementType		getElementType(void) const { return mType; }
		/// Gets the meaning of this element
		VertexElementSemantic	getSemantic(void) const { return mSemantic; }
		/// Gets the index of this element, only applicable for repeating elements
		uint16			getIndex(void) const { return mIndex; }
		/// Gets the size of this element in bytes
		size_t			getSize(void) const;
		// buffer type for multistream rendering.
		VertexBufferType	getBufferType() const { return mBufferType; }
		// step rate for multistream renderint, if the buffer type is VBT_GEOMETRY, this value is surely 0;
		uint32			getInstanceStepRate() const { return mBufferType == VBT_Geometry? 0 : mStepRate; }
		/// query method for helping to calculate offsets
		static size_t	getElementTypeSize(VertexElementType etype);

		bool	operator == (const DVertexElement& ve) const;

	protected:
		// The source vertex buffer,
		// as bound to an index using VertexBufferBinding
		uint16	mSource;
		/// The offset in the buffer that this element starts
		size_t	mOffset;
		/// The type of element
		VertexElementType mType;
		/// The meaning of the element
		VertexElementSemantic mSemantic;
		/// Index of the item, only applicable for some elements like texture coords
		uint16 mIndex;
		// buffer type binded in pipeline.
		VertexBufferType mBufferType;
		// step rate for used in multistream.
		uint32	mStepRate;
	};

	// vertex declaration work as DX's vertex declaration, but
	// it is more convenient and it should be subclassed by 
	// specified render system,
	// NOTICE : you MUST sort() before you send the DVertexDeclaration to the rendering pipeline.!
	class DUEL_API DVertexDeclaration : public DObject
	{
	DUEL_DECLARE_RTTI(DVertexDeclaration)
	public:
		DVertexDeclaration();
		virtual ~DVertexDeclaration();
        /** Adds a new VertexElement to this declaration.
        @remarks
            This method adds a single element (positions, normals etc) to the end of the
            vertex declaration. <b>Please read the information in DVertexDeclaration about
	    the importance of ordering and structure for compatibility with older D3D drivers</b>.
	    @param source The binding index of HardwareVertexBuffer which will provide the source for this element.
			See VertexBufferBindingState for full information.
        @param offset The offset in bytes where this element is located in the buffer
        @param theType The data format of the element (3 floats, a colour etc)
        @param semantic The meaning of the data (position, normal, diffuse colour etc)
        @param index Optional index for multi-input elements like texture coordinates
		@returns A reference to the VertexElement added.
        */
		virtual	const DVertexElement& addElement(uint16 source, size_t offset, 
			VertexElementType type, VertexElementSemantic semantic, uint16 index = 0,
			VertexBufferType bufType = VBT_Geometry, uint32 instanceStepRate = 0);
		/** Inserts a new VertexElement at a given position in this declaration.
        @remarks
        This method adds a single element (positions, normals etc) at a given position in this
        vertex declaration. <b>Please read the information in DVertexDeclaration about
        the importance of ordering and structure for compatibility with older D3D drivers</b>.
        @param source The binding index of HardwareVertexBuffer which will provide the source for this element.
        See VertexBufferBindingState for full information.
        @param offset The offset in bytes where this element is located in the buffer
        @param theType The data format of the element (3 floats, a colour etc)
        @param semantic The meaning of the data (position, normal, diffuse colour etc)
        @param index Optional index for multi-input elements like texture coordinates
        @returns A reference to the VertexElement added.
        */
		virtual const DVertexElement& insertElement(uint32 atPosition, 
			uint16 source, size_t offset, VertexElementType type,
			VertexElementSemantic semantic, uint16 index = 0,
			VertexBufferType bufType = VBT_Geometry, uint32 instanceStepRate = 0);
		/** Remove the element at the given index from this declaration. */
		virtual void	removeElement(uint32 elem_index);
		/** Remove all elements. */
		virtual void	removeAllElement();

		 /** Sorts the elements in this list to be compatible with the maximum
            number of rendering APIs / graphics cards.
        @remarks
            Older graphics cards require vertex data to be presented in a more
            rigid way, as defined in the main documentation for this class. As well
            as the ordering being important, where shared source buffers are used, the
            declaration must list all the elements for each source in turn.
        */
		void			sort();
		// clear all elements.
		void			clear();

		/** Gets the vertex size defined by this declaration for a given source. */
		virtual size_t	getVertexSize(uint16 source);

		/** Get the number of elements in the declaration. */
		size_t			getElementCount(void) { return mVertexElementList.size(); }
		

		typedef std::list<DVertexElement>	VertexElements;
		typedef ListIterator<VertexElements>	VertexElementIterator;
		VertexElementIterator	getVertexElementIterator() { return VertexElementIterator(mVertexElementList.begin(), mVertexElementList.end()); }


	protected:
		VertexElements	mVertexElementList;
	};
}
#endif