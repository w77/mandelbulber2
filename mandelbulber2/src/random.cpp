/**
 * Mandelbulber v2, a 3D fractal generator
 *
 * cRandom class - crossplatform deterministic random generator based on GSL library
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
 * Authors: Sebastian Jennen (jenzebas@gmail.com)
 */

#include "random.hpp"

cRandom::cRandom()
{
	gBaseRand = gsl_rng_alloc(gsl_rng_mt19937);
}

cRandom::~cRandom()
{
	gsl_rng_free(gBaseRand);
}

void cRandom::Initialize(int seed)
{
	gsl_rng_set(gBaseRand, seed);
}


int cRandom::Random(unsigned long max)
{
	return gsl_rng_uniform_int(gBaseRand, max + 1);
}

double cRandom::Random(double min, double max, double resolution)
{
	double range = max - min;
	unsigned long n = round(range / resolution);
	return min + resolution * Random(n);
}

// generates random number with more precision
// works with range / resolution < unsigned long long MAX
double cRandom::DoubleRandom(double min, double max)
{
	double range = max - min;
	double random = gsl_rng_uniform(gBaseRand);
	return min + random * range;
}
