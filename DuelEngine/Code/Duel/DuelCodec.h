//  [8/24/2012 OMEGA] created

#ifndef _DUELCODEC_H_
#define _DUELCODEC_H_

#include "DuelCommon.h"
#include "DuelResource.h"
#include "DuelMemoryStream.h"
#include "DuelFileStream.h"

namespace Duel
{
	// supported code type must add to this enum
	enum CodecType
	{
		CT_Unknown,
		CT_Image,
		CT_Mesh,
		CT_Skeleton,
		CT_RenderEffect,
		CT_Audio
	};
	
	enum CodecFormat
	{
		CF_Unknown,
		CF_DDS
	};

	// interface class, used for data exchanging in code/decode procedure.
	// each codec class should use its own description of the file.
	class DUEL_API DMediaDesc : public DObject
	{
	// use RTTI to ensure we can check class in run-time
	DUEL_DECLARE_RTTI(DMediaDesc)
	public:
		DMediaDesc() : mCodecFormat(CF_Unknown){}
		DMediaDesc(CodecFormat cf) : mCodecFormat(cf){}
		~DMediaDesc(){}

		CodecFormat	getFormat() { return mCodecFormat; }
		void			setFormat(CodecFormat cf) { mCodecFormat = cf; }
	protected:
		// codec may use meta-data to process stream.
		CodecFormat	mCodecFormat;
	};

	// abstract codec class, provide code/decode ability
	// btween two datastream.
	class DUEL_API DMediaCodec : public DObject
	{
	DUEL_DECLARE_RTTI(DMediaCodec)
	public:
		// code a stream and return the result in memory stream type.
		// param input: the stream which contains the data 
		// param codeDesc: the description of the input stream or coding
		// options needed by the coding procedure.
		virtual DDataStreamPtr	encode(DDataStream* input, DMediaDesc* codecDesc) = 0;
		
		// code a stream to given outputFile, make sure the fileName
		// param input: the stream which contains the data 
		// param fileName: use full name: "C:\\xxxx\\ssss.xxx" to specified
		// output file destination.
		// param codeData: the description of the input stream or coding
		// options needed by the coding procedure.
		virtual void			encodeToFile(DDataStream* input, const DString& fileName, DMediaDesc* codecDesc) = 0;

		// return a memory stream as rusult. be careful with memory problem.
		// because it may not release memory when close.
		// param input: the stream which contains the data 
		// param codeDesc(out): the description of the input stream, indicate the result
		// of decoding procedure.
		virtual DDataStreamPtr	decode(DDataStream* input, DMediaDesc* codecDesc) = 0;

		// return the type of the codec
		virtual	CodecType		getCodecType() = 0;
	};
	
	// resource codec only uses in resource's loading.
	class DUEL_API DResourceCodec : public DObject
	{
	DUEL_DECLARE_RTTI(DResourceCodec)
	public:
		// this method is an interface for loading data into resource
		virtual void			decode(DDataStream* input, DResource* targetResource) {}
		// this method is an interface for encoding data into resource
		virtual DDataStreamPtr	encode(DResource* targetResource) { DDataStreamPtr ret; return ret; }
		// this method is an interface for encoding data into file
		virtual void			encodeToFile(const DString& fileName, DResource* targetResource) {}

		// return the type of the codec
		virtual	CodecType		getCodecType() = 0;
	};
}

#endif