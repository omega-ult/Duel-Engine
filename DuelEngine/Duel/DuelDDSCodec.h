//  [8/24/2012 OMEGA] created

#ifndef _DUELDDSCODEC_H_
#define _DUELDDSCODEC_H_

#include "DuelCommon.h"
#include "DuelCodec.h"
#include "DuelImageDesc.h"

namespace Duel
{


	// codec that deal with dds texture format.
	class DUEL_API DDSCodec : public DMediaCodec
	{
	DUEL_DECLARE_RTTI(DDSCodec)
	public:
		// override: DImageCodec----------------
		// code a stream and return the result in memory stream type.
		// the codecDesc should be a DImageDesc.
		DDataStreamPtr	encode(DDataStream* input, DMediaDesc* imageDesc);
		
		// override: DImageCodec----------------
		// code a stream to given outputFile, make sure the fileName
		// use full name: "C:\\xxxx\\ssss.xxx"
		void			encodeToFile(DDataStream* input, const DString& fileName, DMediaDesc* imageDesc);

		// override: DImageCodec----------------
		// return a memory stream as rusult. be careful with memory problem.
		// because it may not release memory when close.
		// NOTICE: the codecDesc that passed in MUST be a DImageDesc!
		DDataStreamPtr	decode(DDataStream* input, DMediaDesc* imageDesc);

		CodecType		getCodecType() { return CT_Image; }
		

		// if we need uncompress DDS file, call this method.
		//MemoryStream*	uncompressDDS();

	};
	

}


#endif