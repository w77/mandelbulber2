/**
 * Mandelbulber v2, a 3D fractal generator
 *
 * auxiliary math functions
 *
 * Copyright (C) 2014 Krzysztof Marczak
 *
 * This file is part of Mandelbulber.
 *
 * Mandelbulber is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Mandelbulber is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with Mandelbulber. If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Krzysztof Marczak (buddhi1980@gmail.com)
 */

#include "common_math.h"

#ifdef __sgi
#include <stdlib.h>
#endif

//********** Random ******************************
int Random(int max)
{
#ifdef WIN32
	return (rand()+rand()*32768) % (max + 1);
#else
	return rand() % (max + 1);
#endif
}

double dMax(double a, double b, double c)
{
	if (a > b)
	{
		if (a > c) return a;
		return c;
	}
	if (b > c) return b;
	return c;
}

double dMin(double a, double b, double c)
{
	if (a < b)
	{
		if (a < c) return a;
		return c;
	}
	if (b < c) return b;
	return c;
}

//----------------------------------------
//reference:
//http://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
//Reflections and Refractions in Ray Tracing
//Bram de Greve (bram.degreve@gmail.com)
//November 13, 2006

CVector3 ReflectionVector(const CVector3 &normal, const CVector3 &incident)
{
	return incident - normal * incident.Dot(normal) * 2.0;
}

CVector3 RefractVector(const CVector3 &normal, const CVector3 &incident, double n1, double n2)
{
	const double n = n1 / n2;
	const double cosI = -normal.Dot(incident);
	const double sinT2 = n * n * (1.0 - cosI * cosI);
	if (sinT2 > 1.0) return CVector3(0.0, 0.0, 0.0); //total internal reflection
	const double cosT = sqrt(1.0 - sinT2);
	return incident * n + normal * (n * cosI - cosT);
}

double Reflectance(const CVector3 &normal, const CVector3 &incident, double n1, double n2)
{
	const double n = n1 / n2;
	const double cosI = -normal.Dot(incident);
	const double sinT2 = n * n * (1.0 - cosI * cosI);
	if (sinT2 > 1.0) return 1.0; //total internal reflection
	const double cosT = sqrt(1.0 - sinT2);
	const double r0rth = (n1 * cosI - n2 * cosT) / (n1 * cosI + n2 * cosT);
	const double rPar = (n2 * cosI - n1 * cosT) / (n2 * cosI + n1 * cosT);
	return (r0rth * r0rth + rPar * rPar) / 2.0;
}

//----------------------------------------

//Smooth transition between two vectors with vector length control
template<typename T>
T SmoothCVector(const T &v1, const T &v2, double k)
{
	T result;
	double nk = 1.0 - k;

	if (k <= 0.0)
	{
		result = v1;
	}
	else if (k >= 1.0)
	{
		result = v2;
	}
	else
	{
		double length1 = v1.Length();
		double length2 = v2.Length();
		double lenInterp = length1 * nk + length2 * k;
		T vTemp = v1 * nk + v2 * k;
		double lengthTemp = vTemp.Length();
		if (lengthTemp > 0.0)
		{
			result = (vTemp / lengthTemp) * lenInterp;
		}
		else
		{
			result = v1;
		}
	}
	return result;
}
template CVector2<double> SmoothCVector(const CVector2<double> &v1, const CVector2<double> &v2,
		double k);
template CVector3 SmoothCVector(const CVector3 &v1, const CVector3 &v2, double k);
template CVector4 SmoothCVector(const CVector4 &v1, const CVector4 &v2, double k);


double cubicInterpolate(double p[4], double x)
{
	return p[1] + 0.5 * x*(p[2] - p[0] + x*(2.0*p[0] - 5.0*p[1] + 4.0*p[2] - p[3] + x*(3.0*(p[1] - p[2]) + p[3] - p[0])));
}

double bicubicInterpolate(double p[4][4], double x, double y)
{
	double yy[4];
	yy[0] = cubicInterpolate(p[0], y);
	yy[1] = cubicInterpolate(p[1], y);
	yy[2] = cubicInterpolate(p[2], y);
	yy[3] = cubicInterpolate(p[3], y);
	return cubicInterpolate(yy, x);
}
