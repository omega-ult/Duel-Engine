// created [10/15/2013 OMEGA]

#include "DuelMPMayaIncludes.h"
#include "DuelMPDMExporter.h"
#include "DuelMPExporter.h"
#include <maya/MFnPlugin.h>


MStatus initializePlugin( MObject obj )
{ 
	MFnPlugin plugin( obj, "DuelExporter", "1.0", "Any");
	MStatus status;

	status = plugin.registerCommand( "DuelExporter",
		DMPExporter::creator);
	if (!status) {
		status.perror("registerCommand");
		return status;
	}
	
	MGlobal::executeCommand("source DuelExporterScript.mel");
	MGlobal::executeCommand("DuelExporter_Install()");

	return status;
}

MStatus uninitializePlugin( MObject obj)
{
	MFnPlugin plugin( obj );
	MStatus   status;
	MGlobal::executeCommand("DuelExporter_Uninstall()");
	status = plugin.deregisterCommand( "DuelExporter" );
	if (!status) {
		status.perror("deregisterCommand");
		return status;
	}


	return status;
}



