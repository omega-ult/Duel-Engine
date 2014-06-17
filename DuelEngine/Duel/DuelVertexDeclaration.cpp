#include "DuelCommon.h"
#include "DuelColor.h"
#include "DuelVertexDeclaration.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DVertexElement, DObject);
	DUEL_IMPLEMENT_RTTI_1(DVertexDeclaration, DObject);

	DVertexElement::DVertexElement( uint16 source, size_t offset, VertexElementType type, 
		VertexElementSemantic semantic, uint16 index /*= 0*/, VertexBufferType bufType,/* = VBT_GEOMETRY*/
		uint32 instanceStepRate/* = 0 */) :
		mSource(source),
		mOffset(offset),
		mType(type),
		mSemantic(semantic),
		mSemanticIndex(index),
		mBufferType(bufType),
		mStepRate(instanceStepRate)
	{

	}

	size_t DVertexElement::getSize( void ) const
	{
		return	getElementTypeSize(mType);
	}
	size_t DVertexElement::getElementTypeSize( VertexElementType etype )
	{
		switch(etype)
		{
		//case VET_COLOR:
		case VET_ABGR:
		case VET_ARGB:
			return sizeof(int8)*4;
		case VET_Byte4:
		case VET_UByte4:
			return sizeof(int8)*4;
		case VET_Float1:
			return sizeof(float);
		case VET_Float2:
			return sizeof(float)*2;
		case VET_Float3:
			return sizeof(float)*3;
		case VET_Float4:
			return sizeof(float)*4;
		case VET_Short1:
		case VET_UShort1:
			return sizeof(int16)*1;
		case VET_Short2:
		case VET_UShort2:
			return sizeof(int16)*2;
		case VET_Short3:
		case VET_UShort3:
			return sizeof(int16)*3;
		case VET_Short4:
		case VET_UShort4:
			return sizeof(int16)*4;
		case VET_Int1:
		case VET_UInt1:
			return sizeof(int32)*1;
		case VET_Int2:
		case VET_UInt2:
			return sizeof(int32)*2;
		case VET_Int3:
		case VET_UInt3:
			return sizeof(int32)*3;
		case VET_Int4:
		case VET_UInt4:
			return sizeof(int32)*4;
		}
		return 0;
	}

	bool DVertexElement::operator==( const DVertexElement& ve ) const
	{
		if (mType != ve.mType ||
			mSemanticIndex != ve.mSemanticIndex ||
			mOffset != ve.mOffset ||
			mSemantic != ve.mSemantic ||
			mSource != ve.mSource)
			return false;
		else
			return true;
	}




	DVertexDeclaration::DVertexDeclaration()
	{
	}

	DVertexDeclaration::~DVertexDeclaration()
	{
	}

	const DVertexElement& DVertexDeclaration::addElement( uint16 source, size_t offset, VertexElementType type, VertexElementSemantic semantic, uint16 index /*= 0*/, VertexBufferType bufType /*= VBT_GEOMETRY*/, uint32 instanceStepRate/* = 0 */)
	{
		mVertexElementList.push_back(
			DVertexElement(source,offset, type, semantic, index, bufType, instanceStepRate));
		return	mVertexElementList.back();
	}

	const DVertexElement& DVertexDeclaration::insertElement( uint32 atPosition, uint16 source, size_t offset, VertexElementType type, VertexElementSemantic semantic, uint16 index /*= 0*/,VertexBufferType bufType /*= VBT_GEOMETRY*/, uint32 instanceStepRate/* = 0 */)
	{
		if(atPosition >= mVertexElementList.size())
		{
			return addElement(source, offset, type, semantic, index, bufType, instanceStepRate);
		}

		VertexElementList::iterator i = mVertexElementList.begin();
		for ( uint32 n = 0; n < atPosition; ++n)
		{
			++i;
		}

		i = mVertexElementList.insert(i,
			DVertexElement(source,offset, type, semantic, index));
		return *i;
	}

	void DVertexDeclaration::removeElement(uint32 elem_index )
	{
		if(elem_index < mVertexElementList.size())
		{
			VertexElementList::iterator i = mVertexElementList.begin();
			for (uint32 n = 0; n < elem_index; ++n)
			{
				++i;
			}
			mVertexElementList.erase(i);
		}
		
	}

	void DVertexDeclaration::removeAllElement()
	{
		mVertexElementList.clear();
	}

	bool vertexElementLess( const DVertexElement& e1, const DVertexElement& e2 )
	{
		// Sort by source first
		if (e1.getSource() < e2.getSource())
		{
			return true;
		}
		else if (e1.getSource() == e2.getSource())
		{
			// Use ordering of semantics to sort
			if (e1.getSemantic() < e2.getSemantic())
			{
				return true;
			}
			else if (e1.getSemantic() == e2.getSemantic())
			{
				// Use index to sort
				if (e1.getSemanticIndex() < e2.getSemanticIndex())
				{
					return true;
				}
			}
		}
		return false;
	}

	void DVertexDeclaration::sort()
	{
		mVertexElementList.sort(vertexElementLess);
	}

	size_t DVertexDeclaration::getVertexSize( uint16 source )
	{
		VertexElementList::const_iterator i, iend;
		iend = mVertexElementList.end();
		size_t sz = 0;

		for (i = mVertexElementList.begin(); i != iend; ++i)
		{
			if (i->getSource() == source)
			{
				sz += i->getSize();

			}
		}
		return sz;
	}

	void DVertexDeclaration::clear()
	{
		mVertexElementList.clear();
	}


// 	const DVertexElement* DVertexDeclaration::getElement(VertexElementSemantic semantic, uint16 source /*= 0 */)
// 	{
// 		VertexElements::iterator i, iend = mVertexElementList.end();
// 		for(i = mVertexElementList.begin(); i != iend; ++i )
// 		{
// 			if (i->getSemantic() == semantic && i->getSource() == source)
// 			{
// 				return &(*i);
// 			}
// 		}
// 		// well we did not find.
// 		assert(false);
// 		return NULL;
// 	}
}