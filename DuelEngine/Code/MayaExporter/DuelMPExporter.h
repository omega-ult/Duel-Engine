//  [1/11/2014 OMEGA] created

#ifndef _DUELMPEXPORTER_H_
#define _DUELMPEXPORTER_H_

#include "DuelMPMayaIncludes.h"
#include "DuelMPExporterParameters.h"
#include "DuelMPMeshData.h"
#include "DuelMPSkeletonData.h"

class DMPExporter : public MPxCommand
{
public:
	DMPExporter();
	// do clean up
	virtual ~DMPExporter();
	//override of MPxCommand methods
	static void* creator();
	MStatus doIt(const MArgList& args);
	bool isUndoable() const;

protected:
	DMPParameters		mParams;

};


#endif