//  [9/17/2012 OMEGA] created

#ifndef _DUELARCHIVEMANAGER_H_
#define _DUELARCHIVEMANAGER_H_

#include "DuelCommon.h"
#include "DuelArchive.h"


namespace Duel
{

	// ArchiveManager holds all registered Archive,
	class DUEL_API DArchiveManager : public DSingleton<DArchiveManager>
	{
	public:
		DArchiveManager();
		DArchiveManager(const DString& configFileName);
		// lalalalalal~~~
		// ArchiveManager(XML);
		virtual ~DArchiveManager();
// 		static ArchiveManager&	getSingleton(void);
// 		static ArchiveManager*	getSingletonPtr(void);

		// used for traverse the Archives.
		typedef	HashMap<DString, ArchiveType>	ArchiveMap;
		typedef	MapIterator<ArchiveMap>			ArchiveMapIterator;
		ArchiveMapIterator	getArchiveIterator() { return ArchiveMapIterator(mArchives.begin(), mArchives.end()); }

		// get an archive from registered archives.
		DArchivePtr			getArchive(const DString& name);

		// add a new archive to current archive map. 
		void				registerArchive(const DString& name, ArchiveType	type);
			
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
	};

}
#endif