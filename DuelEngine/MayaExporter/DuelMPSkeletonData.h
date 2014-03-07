//  [10/24/2013 OMEGA] created

#ifndef _DUELMPSKELETONDATA_H_
#define _DUELMPSKELETONDATA_H_

#include "DuelMPDataTemplate.h"

using std::string;
using std::vector;

class DMPSkeletonData
{
public:
	DMPSkeletonData() {}
	void	clear()
	{
		skeleton.name = "";
		skeleton.subSkeletons.clear();
	}
	typedef	DataTemplate<double, 2>	Point2;
	typedef	DataTemplate<double, 3>	Point3;
	typedef	DataTemplate<double, 4>	Point4;

	struct BoneStruct
	{
		unsigned int	boneHandle;	// internally used, assigned when writing.
		string	name;
		string	parentName;
		Point3	translate;
		Point3	scale;
		Point4	orientation;
	};
	struct TransformKeyFrame
	{
		float	time;
		Point3	translate;
		Point3	scale;
		Point4	orientation;	// order: w,x,y,z
	};
	struct TransformTrack
	{
		string	targetBone;
		vector<TransformKeyFrame>	frames;
	};
	
	struct TransformAnimation
	{
		string	name;
		float	startTime;
		float	endTime;
		vector<TransformTrack>	tracks;
	};

	struct SubSkeletonStruct
	{
		string	name;
		vector<BoneStruct>	bones;
		//////////////////////////////////////////////////////////////////////////
		//  [10/25/2013 OMEGA] todo:
		// add animation.
		vector<TransformAnimation>	animations;
	};

	struct SkeletonStruct
	{
		string name;
		vector<SubSkeletonStruct>	subSkeletons;
	};

	SkeletonStruct	skeleton;
};


#endif