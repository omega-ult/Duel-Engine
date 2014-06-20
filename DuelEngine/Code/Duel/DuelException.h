//  [7/15/2012 OMEGA] created

#ifndef _DUELEXCEPTION_H_
#define _DUELEXCEPTION_H_

#include "DuelCommon.h"
#include <exception>


namespace Duel 
{
	// base exception class
	class DUEL_API DException : std::exception
	{
	public:
		//DException();
		virtual ~DException() {};
		
		DException(int type, const DString& description, const DString& source);
		DException( int type, 
			const DString& description, 
			const DString& fullDesc,
			const DString& source, 
			const char* name, 
			const char* file, 
			long line );

		// if a new exception type added, 
		// insert its type here and define
		// a new subclass
		enum ExceptionType
		{
			ET_FailedToWrite,
			ET_InvalidParameters,
			ET_InvalidStatus,
			ET_NotImplemented,
			ET_ItemNotFound,
			ET_FileNotFound,
			ET_InternalError,
			ET_RenderFailed
		};

		virtual const DString&	getDescription() const { return mDescription; }
		virtual const DString&	getFullDescription() const { return mFullDesc; }
		virtual	int				getType() const  { return mTypeCode; }
		virtual const DString&	getName() const { return mTypeName; }
		virtual const DString&	getSource() const { return mSource; }
		virtual const DString&	getFile() const { return mFile; }
		virtual long			getLine() const { return mLine; }

		const char* what() const  { return getFullDescription().c_str(); }

	protected:
		
		int mTypeCode;
		DString mTypeName;
		DString mDescription;
		DString mSource;
		DString mFile;
		long mLine;
		DString mFullDesc;

	};


	////////////////////////////////////////////////////////////////////
	// implement specified exception
	class DUEL_API IOException : public DException
	{
	public:
		IOException(int type, const DString& description,const DString& fullDesc, const DString& source, const char* file, long line)
			: DException(type, description, fullDesc, source, "IOException", file, line) {}
	};

	class DUEL_API InvalidParametersException : public DException
	{
	public:
		InvalidParametersException(int type, const DString& description,const DString& fullDesc, const DString& source, const char* file, long line)
			: DException(type, description, fullDesc, source, "InvalidParameters", file, line) {}
	};
	
	class DUEL_API InvalidStatusException : public DException
	{
	public:
		InvalidStatusException(int type, const DString& description,const DString& fullDesc, const DString& source, const char* file, long line)
			: DException(type, description, fullDesc, source, "InvalidStatus", file, line) {}
	};

	class DUEL_API NotImplementedException : public DException
	{
	public:
		NotImplementedException(int type, const DString& description,const DString& fullDesc, const DString& source, const char* file, long line)
			: DException(type, description, fullDesc, source, "NotImplemented", file, line) {}
	};

	class DUEL_API ItemNotFoundException : public DException
	{
	public:
		ItemNotFoundException(int type, const DString& description,const DString& fullDesc, const DString& source, const char* file, long line)
			: DException(type, description, fullDesc, source, "ItemNotFound", file, line) {}
	};

	class DUEL_API FileNotFoundException : public DException
	{
	public:
		FileNotFoundException(int type, const DString& description,const DString& fullDesc, const DString& source, const char* file, long line)
			: DException(type, description, fullDesc, source, "FileNotFound", file, line) {}
	};

	class DUEL_API InternalErrorException : public DException
	{
	public:
		InternalErrorException(int type, const DString& description,const DString& fullDesc, const DString& source, const char* file, long line)
			: DException(type, description, fullDesc, source, "InternalError", file, line) {}
	};

	class DUEL_API RenderFailedException : public DException
	{
	public:
		RenderFailedException(int type, const DString& description,const DString& fullDesc, const DString& source, const char* file, long line)
			: DException(type, description, fullDesc, source, "RenderFailed", file, line) {}
	};
	///////////////////////////////////////////////////////////////////



	/** Template struct which creates a distinct type for each exception code.

	This is useful because it allows us to create an overloaded method
	for returning different exception types by value without ambiguity. 
	From 'Modern C++ Design' (Alexandrescu 2001).
	*/
	template <int t>
	struct ExceptionTypeCode
	{
		enum { type = t };
	};


	class ExceptionFactory
	{
	private:
		// no construction allowed
		ExceptionFactory() {};

	public:

		static IOException create(
			ExceptionTypeCode<DException::ET_FailedToWrite> code,
			const DString& description,
			const DString& fullDesc,
			const DString& source,
			const char*	file,
			long line)
		{
			return IOException(code.type, description, fullDesc, source, file, line);
		}

		static ItemNotFoundException create(
			ExceptionTypeCode<DException::ET_ItemNotFound> code,
			const DString& description,
			const DString& fullDesc,
			const DString& source,
			const char*	file,
			long line)
		{
			return ItemNotFoundException(code.type, description, fullDesc, source, file, line);
		}

		static FileNotFoundException create(
			ExceptionTypeCode<DException::ET_FileNotFound> code,
			const DString& description,
			const DString& fullDesc,
			const DString& source,
			const char*	file,
			long line)
		{
			return FileNotFoundException(code.type, description, fullDesc, source, file, line);
		}

		static NotImplementedException create(
			ExceptionTypeCode<DException::ET_NotImplemented> code,
			const DString& description,
			const DString& fullDesc,
			const DString& source,
			const char*	file,
			long line)
		{
			return NotImplementedException(code.type, description, fullDesc, source, file, line);
		}

		static InvalidParametersException create(
			ExceptionTypeCode<DException::ET_InvalidParameters> code,
			const DString& description,
			const DString& fullDesc,
			const DString& source,
			const char*	file,
			long line)
		{
			return InvalidParametersException(code.type, description, fullDesc, source, file, line);
		}

		static InvalidStatusException create(
			ExceptionTypeCode<DException::ET_InvalidStatus> code,
			const DString& description,
			const DString& fullDesc,
			const DString& source,
			const char*	file,
			long line)
		{
			return InvalidStatusException(code.type, description, fullDesc, source, file, line);
		}

		static InternalErrorException create(
			ExceptionTypeCode<DException::ET_InternalError> code,
			const DString& description,
			const DString& fullDesc,
			const DString& source,
			const char*	file,
			long line)
		{
			return InternalErrorException(code.type, description, fullDesc, source, file, line);
		}

		static RenderFailedException create(
			ExceptionTypeCode<DException::ET_RenderFailed> code,
			const DString& description,
			const DString& fullDesc,
			const DString& source,
			const char*	file,
			long line)
		{
			return RenderFailedException(code.type, description, fullDesc, source, file, line);
		}
	};


// use this macro to throw a exception:
#ifndef DUEL_EXCEPT
#define DUEL_EXCEPT(type, desc, fullDesc, src) throw Duel::ExceptionFactory::create( \
	Duel::ExceptionTypeCode<type>(), desc, fullDesc, src, __FILE__, __LINE__ );
#endif

#ifndef DUEL_EXCEPT_BRIEF
#define DUEL_EXCEPT_BRIEF(type, desc, src) throw Duel::ExceptionFactory::create( \
	Duel::ExceptionTypeCode<type>(), desc, desc, src, __FILE__, __LINE__ );
#endif
}


#endif