//  [1/11/2014 OMEGA] created

#include "DuelMPMayaIncludes.h"
#include "DuelMPExporter.h"
#include "DuelMPExporterParameters.h"
#include "DuelMPDMExporter.h"
#include "DuelMPDSExporter.h"

void* DMPExporter::creator()
{
	return new DMPExporter();
}

DMPExporter::DMPExporter()
{

}

DMPExporter::~DMPExporter()
{

}

bool DMPExporter::isUndoable() const
{
	return false;
}

MStatus DMPExporter::doIt( const MArgList& args )
{
	MStatus status;
	mParams = DMPParameters();	// reset parameters.
	status = mParams.parseArgs(args);
	if (status != MStatus::kSuccess)
	{
		return status;
	}
	
	DMPDMExporter dmExporter;
	dmExporter.exportMesh(&mParams);

	DMPDSExporter dsExporter;
	dsExporter.exportSkeleton(&mParams);


	return MStatus::kSuccess;
}
