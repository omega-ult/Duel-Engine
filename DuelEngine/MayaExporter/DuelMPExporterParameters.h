//  [1/12/2014 OMEGA] created

#ifndef _DUELMPEXPORTERPARAMETERS_H_
#define _DUELMPEXPORTERPARAMETERS_H_

#include "DuelMPMayaIncludes.h"
#include <vector>

// 实在没法了. parameter的解析要好长一段代码, 最好不要一起放在其他文件里


// Length units multipliers from Maya internal unit (cm)
#define CM2MM 10.0f
#define CM2CM 1.0f
#define CM2M  0.01f
#define CM2IN 0.393701f
#define CM2FT 0.0328084f
#define CM2YD 0.0109361f

class DMPParameters
{
public:
	DMPParameters();
	MStatus	parseArgs(const MArgList& args);


	enum SkeletonTarget
	{
		ST_UseCurrent,
		ST_UseAssigned,
		ST_NoSkeleton
	};
	enum NeutralPoseType
	{
		NPT_SkinBindPose,
		NPT_CurrentFrame
	};

	enum AnimationSampleType
	{
		AST_Frame,
		AST_Second
	};

	struct AnimationClip
	{
		MString	clipName;
		float	start;
		float	end;
	};

public:
	MString	outputDir;
	bool	bReverseZAxis;
	bool	bExportAll;
	float	lum;	// Length Unit Multiplier
	bool	bExportMesh;
	bool	bExportSkeleton;

	MString	meshFileName;
	bool	bExportMeshNormal;
	SkeletonTarget	skeletonTarget;
	MString	assignSkeleton;
	bool	bExportMorphAnimation;

	MString	skeletonFileName;
	NeutralPoseType	neutralPoseType;
	bool	bExportSkelAnimation;

	AnimationSampleType	animSampleType;
	float	samplerRate;	// for seconds/frames.
	float	fps;	// for frames only.

	std::vector<AnimationClip>	clipList;
};

#endif