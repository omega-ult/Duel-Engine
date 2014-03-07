//  [9/11/2012 OMEGA] created

#ifndef _DUELARCHIVE_H_
#define _DUELARCHIVE_H_

#include "DuelCommon.h"
#include "DuelDataStream.h"
#include "DuelException.h"

namespace Duel
{
	// register new archive type here.
	enum ArchiveType
	{
		AT_Folder,
		AT_Zip,
		AT_WP8AppFolder	// WP8专用标示符.
	};

	// file description, used for communication between 
	// client module and archive.
	struct FileDesc
	{
		// the archive which created this file.
		DArchive*	fatherArchive;
		// the fileName is the full name of the file(with path included).
		DString		fileName;
		// the path is separated by '/' and end with '/'
		DString		path;
		// the baseName is the actural name of the file(without path).
		DString		baseName;
		// compressed size of the file(if it is compressed)
		size_t		compressedSize;
		// original size of the file, when it was not compressed.
		size_t		originalSize;
	};

	typedef	std::vector<FileDesc>	FileDescList;
	typedef	SharedPtr<FileDescList>	FileDescListPtr;

	// an archive is a container of files, it can be a folder or a zip or a certain
	// type of packing format. this class is the base class for subclassed.
	class DUEL_API DArchive : public DObject
	{
	DUEL_DECLARE_RTTI(DArchive)
	public:
		DArchive(const DString& name, ArchiveType type)
			: mName(name), mType(type), mReadOnly(true) {}
		virtual	~DArchive() {}
		// get the name of the archive.
		const DString&	getName(void) const { return mName; }

		// load necessary data for work.
		virtual	void	load() = 0;
		// release data and stop working.
		virtual	void	unload() = 0;
		// if the archive is read-only, then create / remove method will not work
		virtual	bool	isReadOnly() { return mReadOnly; }

		// no close operation provided, the returned stream must handle its lifecycle.
		// fileName must be the full name of the file(with path included),
		// if the archive is read-only, then the readOnly will make no sense, the returned
		// DataStream is surely read-only.
		virtual DDataStreamPtr	open(const DString& fileName, bool readOnly = true) = 0;

		// not all archive support create, if it does support, it must override this method.
		// fileName must be the full name of the file(with path included).
		virtual	DDataStreamPtr	create(const DString fileName)
		{
			DUEL_EXCEPT(DException::ET_NotImplemented,
				"Base class do not support create method",
				"Base class do not support create method, make sure you are using subclass",
				"Duel::DArchive::create")
		}

		// not all archive support remove, if it does support, it must override this method.
		// fileName must be the full name of the file(with path included).
		virtual	void	remove(const DString& fileName)
		{
			DUEL_EXCEPT(DException::ET_NotImplemented,
				"Base class do not support remove method",
				"Base class do not support remove method, make sure you are using subclass",
				"Duel::DArchive::remove")
		}

		// query whether the specified file exist in this archive.
		// fileName must be the full name of the file(with path included).
		virtual	bool	isExisted(const DString& fileName) = 0;

		// list all files' information in this archive.
		// use recursive flag to scan all path included in this archive
		// use showDir to get the directories' description instead of files.
		virtual	FileDescListPtr	getFileDescList(bool recursive = true, bool showDir = false) = 0;

		// list all files' information that match the given pattern in this archive.
		// use recursive flag to scan all path included in this archive
		// use showDir to get the directories' description instead of files.
		virtual	FileDescListPtr	findFile(const DString& pattern, bool recursive = true, bool showDir = false) = 0;

	protected:
		// name of the archive.
		DString		mName;
		// type of the archive.
		ArchiveType	mType;
		// indicate this archive's access
		bool		mReadOnly;
	};
}

#endif