//  [9/27/2012 OMEGA] created

//////////////////////////////////
// rapidxml include
#ifndef _DUELXML_H_
#define _DUELXML_H_
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>

namespace Duel
{
	// notice: xml parsing is destructive if using rapidxml.
	// so copy xml to a new string is a safe way.
#define	DUEL_XML rapidxml
	typedef	DUEL_XML::xml_document<>	DXMLDoc;
	typedef	DUEL_XML::xml_node<>		DXMLNode;
	typedef	DUEL_XML::xml_attribute<>	DXMLAttribute;

	typedef	DUEL_XML::node_type			DXMLNodeType;

	class DUEL_API DXMLTool
	{
	public:
		DXMLTool() {}
		~DXMLTool() {}

		// 加了个trim而已.
		static DString	readValue(DXMLNode* xnode);
		static DString	readValue(DXMLAttribute* xatt);
	};

}
#endif