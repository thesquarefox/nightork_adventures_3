// Copyright (C) 2011-2014 by Maximilian Hönig
// This file is part of "DunGen - the Dungeongenerator".
// For conditions of distribution and use, see licence.txt provided together with DunGen.

#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <irrlicht.h>

/// convert double to integer with rounding up and down
inline int d2i(double d_)
{
	return static_cast<int>(d_<0?d_-0.5:d_+0.5);
}

/// cast float vector into double vector
inline irr::core::vector3d<double> vec3D(const irr::core::vector3df& inVec_)
{
	return irr::core::vector3d<double>(static_cast<double>(inVec_.X),
		static_cast<double>(inVec_.Y), static_cast<double>(inVec_.Z));
}

/// cast double vector into float vector
inline irr::core::vector3df vec3F(const irr::core::vector3d<double>& inVec_)
{
	return irr::core::vector3df(static_cast<irr::f32>(inVec_.X),
		static_cast<irr::f32>(inVec_.Y), static_cast<irr::f32>(inVec_.Z));
}

/// compute weight values for each point of a triangle, dependent on angle in this point
///
/// source: CMeshManipulator.cpp (Irrlicht Release 1.7.3) - function getAngleWeight()
/// slightly modified for double precision
inline irr::core::vector3d<double> computeAngleWeight(
	const irr::core::vector3d<double>& v1_,
	const irr::core::vector3d<double>& v2_,
	const irr::core::vector3d<double>& v3_)
{	
	// compute the side lenght of each side
	double a = v2_.getDistanceFromSQ(v3_);
	double asqrt = sqrt(a);
	double b = v1_.getDistanceFromSQ(v3_);
	double bsqrt = sqrt(b);
	double c = v1_.getDistanceFromSQ(v2_);
	double csqrt = sqrt(c);

	// compute the angle in each point
	return irr::core::vector3d<double>(
		acos((b + c - a) / (2.0 * bsqrt * csqrt)),
		acos((-b + c + a) / (2.0 * asqrt * csqrt)),
		acos((b - c + a) / (2.0 * bsqrt * asqrt)));
}

#endif