/**
 * Mandelbulber v2, a 3D fractal generator
 *
 * Compute - function fractal computation
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

#ifndef COMPUTE_FRACTAL_HPP_
#define COMPUTE_FRACTAL_HPP_

#include "commonparams.hpp"
#include "fractal_coloring.hpp"
#include "nine_fractals.hpp"

struct sFractalIn
{
	CVector3 point;
	int minN;
	int maxN;
	sCommonParams common;
	int forcedFormulaIndex;
	sFractalColoring fractalColoring;
	sFractalIn(CVector3 _point, int _minN, int _maxN, const sCommonParams &_common,
			int _forcedFormulaIndex, const sFractalColoring &_fractalColoring = sFractalColoring()) :
					point(_point),
					minN(_minN),
					maxN(_maxN),
					common(_common),
					forcedFormulaIndex(_forcedFormulaIndex),
					fractalColoring(_fractalColoring)
	{
	}

};

struct sFractalOut
{
	CVector3 z;
	double distance;
	double colorIndex;
	double fakeAO;
	double orbitTrapR;
	int iters;
	bool maxiter;
};

template<fractal::enumCalculationMode Mode>
void Compute(const cNineFractals &fractals, const sFractalIn &in, sFractalOut *out);

#endif /* COMPUTE_FRACTAL_HPP_ */
