//  [9/14/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelFolderArchive.h"
#include "DuelFileStream.h"
#include "DuelException.h"
#include "DuelLogManager.h"

#ifdef DUEL_PLATFORM_WINDOWS
#include <direct.h>

#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
using namespace Microsoft::WRL;
using namespace Windows::Storage;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Streams;
using namespace Windows::ApplicationModel;
#include <fileapi.h>
#endif

#endif

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DFolderArchive, DArchive)
	//-----------------------------------------------------------------------
	// utility function
	static bool is_reserved_dir (const char *fn)
	{
		return (fn [0] == '.' && (fn [1] == 0 || (fn [1] == '.' && fn [2] == 0)));
	}
	//-----------------------------------------------------------------------
	static bool is_absolute_path(const char* path)
	{
#ifdef	DUEL_PLATFORM_WINDOWS
		if (isalpha(unsigned char(path[0])) && path[1] == ':')
			return true;
#endif
		return path[0] == '/' || path[0] == '\\';
	}
	//-----------------------------------------------------------------------
	static DString concatenate_path(const DString& base, const DString& name)
	{
		if (base.empty() || is_absolute_path(name.c_str()))
			return name;
		else
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
			return base + '\\' + name;
#else
			return base + '/' + name;
#endif // DUEL_PLATFORM_WINDOWS_PHONE_8
	}
	//----------------------------------------------------------------------
	void DFolderArchive::findFiles(const DString& pattern, bool recursive, bool showDir, FileDescList* outList)
	{
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
		HANDLE	lHandle;
		BOOL res;
		WIN32_FIND_DATA	tagData;

		// pattern can contain a directory name, separat it from mask
		size_t pos1	= pattern.rfind('/');
		size_t pos2 = pattern.rfind('\\');
		if(pos1 == pattern.npos || ((pos2 != pattern.npos) && (pos1 < pos2)))
		{
			pos1 = pos2;
		}
		DString directory;
		if(pos1 != pattern.npos)
		{
			directory = pattern.substr(0, pos1 + 1);
		}

		DWString tmp;
		DString full_pattern = concatenate_path(mName, pattern);
		DStringTool::convert(tmp, full_pattern);

		lHandle = FindFirstFileEx(
			tmp.c_str(),
			FindExInfoStandard,
			&tagData,
			FindExSearchNameMatch,
			NULL,
			0);
		res = TRUE;

		while(lHandle != INVALID_HANDLE_VALUE && res != false)
		{
			DString fNameTmp;
			DStringTool::convert(fNameTmp, DWString(tagData.cFileName));
			if((showDir == ((tagData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)) &&
				(!mIgnoreHidden || (tagData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0) &&
				(!showDir || !is_reserved_dir(fNameTmp.c_str())))
			{
				FileDesc fd;
				fd.fatherArchive = this;
				fd.fileName = directory + fNameTmp;
				fd.baseName	= fNameTmp;
				fd.path		= directory;
				fd.compressedSize	= tagData.nFileSizeLow;
				fd.originalSize		= tagData.nFileSizeLow;
				outList->push_back(fd);
			}
			res = FindNextFile(lHandle, &tagData);
		}			// close if we found any files
		if(lHandle != INVALID_HANDLE_VALUE)
		{
			FindClose(lHandle);
		}
#else
		long lHandle, res;
		struct _finddata_t tagData;

		// pattern can contain a directory name, separat it from mask
		size_t pos1	= pattern.rfind('/');
		size_t pos2 = pattern.rfind('\\');
		if(pos1 == pattern.npos || ((pos2 != pattern.npos) && (pos1 < pos2)))
		{
			pos1 = pos2;
		}
		DString directory;
		if(pos1 != pattern.npos)
		{
			directory = pattern.substr(0, pos1 + 1);
		}

		DString full_pattern = concatenate_path(mName, pattern);

		lHandle = _findfirst(full_pattern.c_str(), &tagData);
		res = 0;
		while(lHandle != -1 && res != -1)
		{
			if((showDir == ((tagData.attrib & _A_SUBDIR) != 0)) &&
				(!mIgnoreHidden || (tagData.attrib & _A_HIDDEN) == 0) &&
				(!showDir || !is_reserved_dir(tagData.name)))
			{
				FileDesc fd;
				fd.fatherArchive = this;
				fd.fileName = directory + tagData.name;
				fd.baseName	= tagData.name;
				fd.path		= directory;
				fd.compressedSize	= tagData.size;
				fd.originalSize		= tagData.size;
				outList->push_back(fd);
			}
			res = _findnext(lHandle, &tagData);
		}
		// Close if we found any files
		if(lHandle != -1)
		{
			_findclose(lHandle);
		}

		// now find directories
		if(recursive)
		{
			DString base_dir = mName;
			if(!directory.empty())
			{
				base_dir = concatenate_path(mName,directory);
				// remove the last '/'
				base_dir.erase(base_dir.length() -1);
			}
			base_dir.append("/*");

			// remove directory name from pattern
			DString mask("/");
			if(pos1 != pattern.npos)
			{
				mask.append(pattern.substr(pos1 + 1));
			}
			else
			{
				mask.append(pattern);
			}

			lHandle = _findfirst(base_dir.c_str(), &tagData);
			res = 0;
			while(lHandle != -1 && res != -1)
			{
				if((tagData.attrib & _A_SUBDIR) &&
					(!mIgnoreHidden || (tagData.attrib & _A_HIDDEN) == 0) &&
					!is_reserved_dir(tagData.name))
				{
					// recurse
					base_dir = directory;
					base_dir.append(tagData.name).append(mask);
					findFiles(base_dir, recursive, showDir, outList);
				}
				res = _findnext(lHandle, &tagData);
			}
			// close if we found any files
			if(lHandle != -1)
			{
				_findclose(lHandle);
			}
		}
#endif // DUEL_PLATFORM_WINDOWS_PHONE_8

	}


	
	DFolderArchive::DFolderArchive( const DString& name, ArchiveType type ) :
		mIgnoreHidden(false),
		DArchive(name, type)
	{
	}

	DFolderArchive::~DFolderArchive()
	{
		unload();
	}

	void DFolderArchive::load()
	{
		DUEL_LOCK_AUTO_MUTEX

		DWString	tmp;
		if(mType == AT_WP8AppFolder)
		{
#ifdef DUEL_PLATFORM_WINDOWS_8
			Windows::Storage::StorageFolder^ systemLocation = Package::Current->InstalledLocation;
			Platform::String^ systemLocationPath = Platform::String::Concat(systemLocation->Path, "\\");

			// - -Äã¶®µÄ......
			DStringTool::convert(tmp, mName);
			Platform::String^ fn = ref new Platform::String(tmp.c_str(), tmp.length());
			fn = Platform::String::Concat(systemLocationPath, fn);
			tmp = DWString(fn->Begin(), fn->End());
			DStringTool::convert(mName,tmp);
#endif

		}
		else
		{
#ifdef DUEL_PLATFORM_WINDOWS_PHONE_8
			Windows::Storage::StorageFolder^ localFolder = ApplicationData::Current->LocalFolder;
			Platform::String^ localFolderPath = Platform::String::Concat(localFolder->Path, "\\");

			// - -Äã¶®µÄ......
			DStringTool::convert(tmp, mName);
			Platform::String^ fn = ref new Platform::String(tmp.c_str(), tmp.length());
			fn = Platform::String::Concat(localFolderPath, fn);
			tmp = DWString(fn->Begin(), fn->End());
			DStringTool::convert(mName,tmp);
#endif // DUEL_PLATFORM_WINDOWS_PHONE_8

		}


			
		DString	writeTest = concatenate_path(mName, "_write");
		std::ofstream	test(writeTest.c_str());
		if(test.fail())
		{
			mReadOnly = true;
		}
		else
		{
			mReadOnly = false;
			test.close();
			::remove(writeTest.c_str());
		}
	}

	void DFolderArchive::unload()
	{
		// ah, nothing to do
	}

	DDataStreamPtr DFolderArchive::open( const DString& fileName, bool readOnly /*= true*/ )
	{
		DString fullName = concatenate_path(mName, fileName);

		DFileStream*	retFS = NULL;

		if (!isExisted(fileName))
		{
			DLogManager::getSingleton().logMessage("Specified file : " 
				+ fullName + "does not exist.");
		}
			
		// if we can open on a ios::out mode
		if(!readOnly && !mReadOnly)
		{
			retFS = new DFileStream(fullName, DA_Read);
		}
		else
		{
			retFS = new DFileStream(fullName, DA_Write);
		}

		return	DDataStreamPtr(retFS);
	}

	DDataStreamPtr DFolderArchive::create( const DString fileName )
	{
		if(mReadOnly)
		{
			DUEL_EXCEPT(DException::ET_InvalidParameters,
				"Creation is denied in this archive",
				"This archive is read-only",
				"Duel::DFolderArchive::create")
		}
		DString	fullName = concatenate_path(mName,fileName);

		DFileStream*	retFS = new DFileStream(fullName, DA_Write);

		return	DDataStreamPtr(retFS);
	}

	void DFolderArchive::remove( const DString& fileName )
	{
		if(mReadOnly)
		{
			DUEL_EXCEPT(DException::ET_InvalidParameters,
				"Cannot remove a file from a read-only archive",
				"Cannot remove a file from a read-only archive",
				"Duel::DFolderArchive::remove");
		}
		DString	fullName = concatenate_path(mName, fileName);
		::remove(fullName.c_str());
	}
	
	bool DFolderArchive::isExisted( const DString& fileName )
	{

		DString fullName = concatenate_path(mName, fileName);
		// c stdlib is helpful.
		if(_access(fullName.c_str(), 0) == 0)
		{
			return	true;
		}
		return false;
	}

	FileDescListPtr DFolderArchive::getFileDescList( bool recursive /*= true*/, bool showDir /*= false*/ )
	{
		
		FileDescListPtr retFD(new FileDescList());
		findFiles("*", recursive, showDir, retFD.get());
		return	retFD;
	}

	FileDescListPtr DFolderArchive::findFile( const DString& pattern, bool recursive /*= true*/, bool showDir /*= false*/ )
	{
		FileDescListPtr retFD(new FileDescList());
		findFiles(pattern, recursive, showDir, retFD.get());
		return	retFD;
	}


}