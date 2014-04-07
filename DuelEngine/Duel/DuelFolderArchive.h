//  [9/14/2012 OMEGA] created

#ifndef _DUELFOLDERARCHIVE_H_
#define _DUELFOLDERARCHIVE_H_

#include "DuelCommon.h"
#include "DuelArchive.h"

namespace Duel
{

	// a folder archive is an archive communicate to the OS's file system,
	class DUEL_API DFolderArchive : public DArchive
	{
	DUEL_DECLARE_RTTI(DFolderArchive)
	public:
		// type 可以是一般的AT_FOLDER 或者是 AT_APPFOLDER_WP8, 应对不同机制.
		DFolderArchive(const DString& name);
		virtual ~DFolderArchive();
		// override : DArchive--------------------------------------
		// load necessary data for work.
		void	load();
		// override : DArchive--------------------------------------
		// release data and stop working.
		void	unload();

		// override : DArchive--------------------------------------
		// no close operation provided, the returned stream must handle its lifecycle.
		// fileName must be the full name of the file(with path included),
		// if the archive is read-only, then the readOnly will make no sense, the returned
		// DataStream is surely read-only.
		DDataStreamPtr	open(const DString& fileName, bool readOnly = true);

		// override : DArchive--------------------------------------
		// create a new file, store it in this archive.
		DDataStreamPtr	create(const DString fileName);

		// override : DArchive--------------------------------------
		// remove an existing file from this archive.
		void	remove(const DString& fileName);

		// override : DArchive--------------------------------------
		// query whether a file exist here.
		bool	isExisted(const DString& fileName);

		// override : DArchive--------------------------------------
		// list all files' information in this archive.
		// use recursive flag to scan all path included in this archive
		// use showDir to get the directories' description instead of files.
		FileDescListPtr	getFileDescList(bool recursive = true, bool showDir = false);

		// override : DArchive--------------------------------------
		// list all files' information that match the given pattern in this archive.
		// use recursive flag to scan all path included in this archive
		// use showDir to get the directories' description instead of files.
		FileDescListPtr	findFile(const DString& pattern, bool recursive = true, bool showDir = false);

		// set whether we should operate on hidden file.
		void	setIgnoreHidden(bool flag) { mIgnoreHidden = flag; }

		// if it returns true, the hidden file will not be listed.
		bool	isIgnoreHidden() { return mIgnoreHidden; }


	protected:
		// used for thread safe.
		DUEL_AUTO_MUTEX
			// utility function for find files.
			void	findFiles(const DString& pattern, bool recursive, bool showDir, FileDescList* outList);

		// indicate whether we should operate on hidden file.
		bool	mIgnoreHidden;

		
	};

}
#endif