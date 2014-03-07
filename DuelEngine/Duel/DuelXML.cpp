//  [3/24/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelString.h"
#include "DuelXML.h"

namespace Duel
{


	DString	DXMLTool::readValue(DXMLNode* xnode)
	{
		DString	ret;
		if (xnode == NULL)
		{
			return ret;
		}
		ret.assign(xnode->value());
		DStringTool::trim(ret, true, true);
		return ret;
	}

	DString DXMLTool::readValue( DXMLAttribute* xatt )
	{
		DString	ret;
		if (xatt == NULL)
		{
			return ret;
		}
		ret.assign(xatt->value());
		DStringTool::trim(ret, true, true);
		return ret;
	}

}