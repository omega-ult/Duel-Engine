//  [9/17/2012 OMEGA] created

#ifndef _DUELARCHIVEMANAGER_H_
#define _DUELARCHIVEMANAGER_H_

#include "DuelCommon.h"
#include "DuelArchive.h"
#include "DuelFolderArchive.h"

namespace Duel
{
	// plugin can provide other types of archive, so the creation of an archive
	// should be done in a factory.
	class DUEL_API DArchiveFactory : public DObject
	{
		DUEL_DECLARE_RTTI(DArchiveFactory)
	public:
		DArchiveFactory(const DString& archType);
		const DString&	getArchiveType();

		virtual DArchivePtr	getArchive(const DString& name) = 0;

	protected:
		DString		mArchType;
	};
	// for default folder archive.
	class DUEL_API DFolderArchiveFactory : public DArchiveFactory
	{
		DUEL_DECLARE_RTTI(DFolderArchiveFactory);
	public:
		DFolderArchiveFactory();

		virtual DArchivePtr getArchive( const DString& name );

	};

	// ArchiveManager holds all registered Archive,
	class DUEL_API DArchiveManager : public DSingleton<DArchiveManager>
	{
	public:
		DArchiveManager();
		DArchiveManager(const DString& configFileName);
		~DArchiveManager();
		// register an archive factory.
		void				registerArchiveFactory(DArchiveFactory* af);
		void				unregisterArchiveFactory(DArchiveFactory* af);

		// used for traverse the Archives.
		// key - archive name, value - archive type.
		typedef	HashMap<DString, DString>	ArchiveMap;
		typedef	MapIterator<ArchiveMap>		ArchiveMapIterator;
		ArchiveMapIterator	getArchiveIterator() { return ArchiveMapIterator(mArchives); }

		// get an archive from registered archives.
		DArchivePtr			getArchive(const DString& name);

		// add a new archive to current archive map. 
		void				registerArchive(const DString& name, const DString& type);
			
		// use XML to load Archives
		// the XML should look like:
		// <ArchiveList>
		//		<Archive>
		//			<Name>lalalala</Name>
		//			<Type>folder</Type>
		//		</Archive>
		//		<Archive>
		//			<Name>dadadada</Name>
		//			<Type>zip</Type>
		//		</Archive>
		//		.......
		// </ArchiveList>
		// supported types are "folder", "zip"......
		

		// using specified config file name to register Archives
		void		registerFromXML(const DString& configFileName);
		// the xmlData should be valid xml document.
		void		registerFromXML(DDataStreamPtr& xmlData);
		// use a given Node to register Archives, notice,
		// the startNode should have name "ArchiveList"
		void		registerFromXML(DXMLNode* startNode);

	protected:
		ArchiveMap	mArchives;
		typedef		std::map<DString, DArchiveFactory*>	ArchiveFactoryMap;
		ArchiveFactoryMap	mArchFactoryMap;

	private:
		// default folder archive factory, for folders only.
		DFolderArchiveFactory*	mFolderFactory;
	};

}
#endif