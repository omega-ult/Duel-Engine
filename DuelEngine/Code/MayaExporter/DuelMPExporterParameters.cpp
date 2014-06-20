//  [1/12/2014 OMEGA] created

#include "DuelMPMayaIncludes.h"
#include "DuelMPExporterParameters.h"


DMPParameters::DMPParameters() :
	bReverseZAxis(false),
	bExportAll(false),
	bExportMesh(false),
	bExportSkeleton(false),
	bExportSkelAnimation(false),
	bExportMeshNormal(false),
	bExportMorphAnimation(false),
	samplerRate(1.0f),
	skeletonTarget(ST_NoSkeleton),
	neutralPoseType(NPT_SkinBindPose),
	animSampleType(AST_Frame),
	fps(24.0f)
{

}

MStatus DMPParameters::parseArgs( const MArgList& args )
{
	MStatus stat;
	// reset all parameters.
	*this = DMPParameters();
	// param list
	MString	output = "-output";
	bool exportTargetSpecified = false;
	MString	all = "-all";
	MString sel = "-sel";
	MString lu = "-lu";
	MString revZ = "-revZ";
	MString mesh = "-mesh";
	MString norm = "-norm";
	MString curSkel = "-curSkel";
	MString assSkel = "-assSkel";
	MString morphAnim = "-morphAnim";
	MString skel = "-skel";
	MString np = "-np";
	MString anim = "-skelAnim";
	MString samp = "-samp";
	MString fps = "-fps";
	MString sampRate = "-sampRate";
	MString clip = "-clip";
	MString range = "-range";
	MString curArg;
	// Parse arguments from command line
	for (unsigned int i = 0; i < args.length(); i++)
	{
		curArg = args.asString(i,&stat);
		if (output == curArg && (MS::kSuccess == stat))
		{
			++i;
			outputDir = args.asString(i, &stat);
			if (!((MS::kSuccess == stat) &&
				outputDir.substring(outputDir.length()-1, outputDir.length()) == "/"))
			{
				MGlobal::executeCommand("print \"invalid parameter of -output\\n\"");
				stat.perror("invalid parameter of -output");
				return stat;
			}
		}
		else if (all == curArg && (MS::kSuccess == stat))
		{
			exportTargetSpecified = true;
			bExportAll = true;
		}
		else if (sel == curArg && (MS::kSuccess == stat))
		{
			exportTargetSpecified = true;
			bExportAll = false;
		}
		else if (lu == curArg && (MS::kSuccess == stat))
		{
			++i;
			MString unit = args.asString(i, &stat);
			if (!(MS::kSuccess == stat))
			{
				MGlobal::executeCommand("print \"invalid parameter of -lu\\n\"");
				stat.perror("invalid parameter of -lu");
				return stat;
			}
			if (MString("pref") == unit)
			{
				MGlobal::executeCommand("currentUnit -q -l",unit,false);
			}
			if (MString("mm") == unit)
			{
				lum = CM2MM;
			}
			else if (MString("cm") == unit)
			{
				lum = CM2CM;
			}
			else if (MString("m") == unit)
			{
				lum = CM2M;
			}
			else if (MString("in") == unit)
			{
				lum = CM2IN;
			}
			else if (MString("ft") == unit)
			{
				lum = CM2FT;
			}
			else if (MString("yd") == unit)
			{
				lum = CM2YD;
			}
			else
			{
				stat.perror("unknown parameter: " + unit);
				MGlobal::executeCommand("print \"invalid parameter of -lu\\n\"");
				return stat;
			}
		}
		else if (revZ == curArg && (MS::kSuccess == stat))
		{
			bReverseZAxis = true;
		}
		else if (mesh == curArg && (MS::kSuccess == stat))
		{
			bExportMesh = true;
			++i;
			meshFileName = args.asString(i, &stat);
			if (!((MS::kSuccess == stat) && meshFileName.substring(0,1) != "-"))
			{
				MGlobal::executeCommand("print \"invalid parameter of -mesh\\n\"");
				stat.perror("invalid parameter of -mesh");
				return stat;
			}
		}
		else if (norm == curArg && (MS::kSuccess == stat))
		{
			bExportMeshNormal = true;
		}
		else if (assSkel == curArg && (MS::kSuccess == stat))
		{
			++i;
			assignSkeleton = args.asString(i, &stat);
			if (!((MS::kSuccess == stat) && assignSkeleton.substring(0,1) != "-"))
			{
				MGlobal::executeCommand("print \"invalid parameter of -assSkel\\n\"");
				stat.perror("invalid parameter of -assSkel");
				return stat;
			}
			skeletonTarget = ST_UseAssigned;
		}
		else if (curSkel == curArg && (MS::kSuccess == stat))
		{
			skeletonTarget = ST_UseCurrent;
		}
		else if (morphAnim == curArg && (MS::kSuccess == stat))
		{
			bExportMorphAnimation = true;
		}
		else if (skel == curArg && (MS::kSuccess == stat))
		{
			bExportSkeleton = true;
			++i;
			skeletonFileName = args.asString(i, &stat);
			if (!((MS::kSuccess == stat) && skeletonFileName.substring(0,1) != "-"))
			{
				MGlobal::executeCommand("print \"invalid parameter of -skel\\n\"");
				stat.perror("invalid parameter of -skel");
				return stat;
			}
		}
		else if (np == curArg && (MS::kSuccess == stat))
		{
			++i;
			MString npType = args.asString(i, &stat);
			if (!((MS::kSuccess == stat) && skeletonFileName.substring(0,1) != "-"))
			{
				MGlobal::executeCommand("print \"invalid parameter of -np\\n\"");
				stat.perror("invalid parameter of -np");
				return stat;
			}
			if (npType == "bindPose")
			{
				neutralPoseType = NPT_SkinBindPose;
			}
			else if (npType == "curFrame")
			{
				neutralPoseType = NPT_CurrentFrame;
			}
			else
			{
				MGlobal::executeCommand("print \"invalid parameter of -np\\n\"");
				stat.perror("invalid parameter of -np");
				return stat;
			}
		}
		else if (anim == curArg && (MS::kSuccess == stat))
		{
			bExportSkelAnimation = true;
		}
		else if (samp == curArg && (MS::kSuccess == stat))
		{
			++i;
			MString sampType = args.asString(i, &stat);
			if (!((MS::kSuccess == stat) && skeletonFileName.substring(0,1) != "-"))
			{
				MGlobal::executeCommand("print \"invalid parameter of -samp\\n\"");
				stat.perror("invalid parameter of -samp");
				return stat;
			}
			if (sampType == "frame")
			{
				animSampleType = AST_Frame;
			}
			else if (sampType == "second")
			{
				animSampleType = AST_Second;
			}
			else
			{
				MGlobal::executeCommand("print \"invalid parameter of -samp\\n\"");
				stat.perror("invalid parameter of -samp");
				return stat;
			}
		}
		else if (fps == curArg && (MS::kSuccess == stat))
		{
			++i;
			fps = (float)args.asDouble(i, &stat);
			if (!(MS::kSuccess == stat))
			{
				MGlobal::executeCommand("print \"invalid parameter of -fps\\n\"");
				stat.perror("invalid parameter of -fps");
				return stat;
			}

		}
		else if (sampRate == curArg && (MS::kSuccess == stat))
		{
			++i;
			samplerRate = (float)args.asDouble(i, &stat);
			if (!(MS::kSuccess == stat))
			{
				MGlobal::executeCommand("print \"invalid parameter of -sampRate\\n\"");
				stat.perror("invalid parameter of -sampRate");
				return stat;
			}
		}
		else if (clip == curArg && (MS::kSuccess == stat))
		{
			++i;
			AnimationClip newClip;
			newClip.clipName = args.asString(i, &stat);
			if(!((MS::kSuccess == stat) && skeletonFileName.substring(0,1) != "-"))
			{
				MGlobal::executeCommand("print \"invalid parameter of -clip\\n\"");
				stat.perror("invalid parameter of -clip");
				return stat;
			}

			++i;
			if (MString("-range") == args.asString(i,&stat) && (MS::kSuccess == stat))
			{
				++i;
				newClip.start = (float)args.asDouble(i, &stat);
				if (!(MS::kSuccess == stat))
				{
					MGlobal::executeCommand("print \"invalid parameter of -range\\n\"");
					stat.perror("invalid parameter of -range");
					return stat;
				}
				++i;
				newClip.end = (float)args.asDouble(i, &stat);
				if (!(MS::kSuccess == stat))
				{
					MGlobal::executeCommand("print \"invalid parameter of -range\\n\"");
					stat.perror("invalid parameter of -range");
					return stat;
				}
				clipList.push_back(newClip);
			}
			else
			{
				MGlobal::executeCommand("print \"incomplete parameter of -range\\n\"");
				stat.perror("incomplete parameter of -range");
				return stat;
			}
		}
		else
		{
			stat.perror("unknown parameter");
			MGlobal::executeCommand("print \"unknown parameter: " + curArg + "\\n\"");
			return stat;
		}
	}
	return stat;
}


