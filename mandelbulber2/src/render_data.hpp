/**
 * Mandelbulber v2, a 3D fractal generator
 *
 * sRenderData struct - container for auxiliary data
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

#ifndef RENDER_DATA_HPP_
#define RENDER_DATA_HPP_

#include "lights.hpp"
#include "region.hpp"
#include "statistics.h"
#include "rendering_configuration.hpp"
#include "material.h"
#include "object_data.hpp"
#include "texture.hpp"

struct sTextures
{
	cTexture backgroundTexture;
	cTexture envmapTexture;
	cTexture lightmapTexture;
	QList<cTexture*> textureList;

	sTextures()
	{
		textureList.append(&backgroundTexture);
		textureList.append(&envmapTexture);
		textureList.append(&lightmapTexture);
	}
	;
};

struct sRenderData
{
	sRenderData() :
			rendererID(0), stopRequest(NULL), lastPercentage(1.0), reduceDetail(1.0)
	{
	}

	int rendererID;
	cRegion<int> screenRegion;
	cRegion<double> imageRegion;
	sTextures textures;
	cLights lights;
	bool *stopRequest;
	double lastPercentage;
	double reduceDetail;
	cStatistics statistics;
	QList<int> netRenderStartingPositions;
	cRenderingConfiguration configuration;

	QMap<int, cMaterial> materials; // 'int' is an ID
	QVector<cObjectData> objectData;
};

#endif /* RENDER_DATA_HPP_ */
