//  [11/4/2012 OMEGA] created

#ifndef _DUELSPLINE_H_
#define _DUELSPLINE_H_

#include <vector>
#include "DuelVector.h"

namespace Duel
{

	// a class defines a spline 
	class DUEL_API DSpline
	{
	public:
		DSpline();
		~DSpline();

		// add a control point to the end of the spline.
		void			addPoint(const DVector3& p);
		// get a point in specified inde.		
		const DVector3&	getPoint(size_t index);
		// get the count of pointers.
		size_t			getPointCount();
		// remove a point in specified index, if it exist.
		void			removePoint(size_t index);
		// remove all points
		void			clear();

		// set a new value to the point in specified index, if it exist
		void			setPoint(size_t index, const DVector3& p);

		// Given a t value between 0 and 1 representing the parametric distance along the
		// whole length of the spline, this method returns an interpolated point.
		DVector3			interpolate(DReal t) const;
		// Interpolates a single segment of the spline given a parametric value.
		DVector3			interpolateFrom(size_t index, DReal t) const;

		// Tells the spline whether it should automatically update tangents on demand
		//	as points are added.
		void			setAutoUpdate(bool flag);
		// Recalculates the tangents associated with this spline.
		void			update();

	protected:
		// control point list
		std::vector<DVector3>	mPoints;
		std::vector<DVector3>	mTangents;
		// indicate whether we are using autoupdate.
		bool					mbAutoUpdate;
		// flag indicate whether we need to update.
		bool					mbDirty;
		// coefficitent that used to calculate spline.
		const static DMatrix4	Coefficient;
	};

	// a spline that represent rotation.
	class DUEL_API RotationSpline
	{
	public:
		RotationSpline();
		~RotationSpline();

		// add a control point to the end of the spline.
		void				addPoint(const DQuaternion& p);

		// get a point in specified
		const DQuaternion&	getPoint(uint16 index) const;

		// get the count of pointers.
		uint16				getPointCount() const;
		// remove a point in specified index, if it exist.
		void				removePoint(size_t index);
		// remove all points
		void				clear();

		// set a new value to the point in specified index, if it exist
		void				setPoint(size_t index, const DQuaternion& p);

		// Given a t value between 0 and 1 representing the parametric distance along the
		// whole length of the spline, this method returns an interpolated point.
		// set nearest to true to use nearest quaternion interpolate
		DQuaternion			interpolate(DReal t, bool nearest = true) const;
		// Interpolates a single segment of the spline given a parametric value.
		// set nearest to true to use nearest quaternion interpolate
		DQuaternion			interpolateFrom(size_t index, DReal t, bool nearest = true) const;

		// Tells the spline whether it should automatically update tangents on demand
		//	as points are added.
		void				setAutoUpdate(bool flag);
		// Recalculates the tangents associated with this spline.
		void				update();

	protected:
		// control point list
		std::vector<DQuaternion>	mPoints;
		std::vector<DQuaternion>	mTangents;
		// indicate whether we are using autoupdate.
		bool					mbAutoUpdate;
		// flag indicate whether we need to update.
		bool					mbDirty;
	};

}


#endif