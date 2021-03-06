/**
 * Mandelbulber v2, a 3D fractal generator
 *
 * cParamRender class - container for scene parameters
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

#ifndef FRACTPARAMS_HPP_
#define FRACTPARAMS_HPP_

#include "parameters.hpp"
#include "fractal.h"
#include "fractal_container.hpp"
#include "object_data.hpp"
#include "image_adjustments.h"
#include "primitives.h"
#include "commonparams.hpp"
#include "projection_3d.hpp"

namespace params
{
enum enumAOMode
{
	AOmodeFast = 0, AOmodeMultipeRays = 1, AOmodeScreenSpace = 2
};

enum enumTextureMapType
{
	mapEquirectangular = 0, mapDoubleHemisphere = 1, mapFlat = 2
};

enum enumBooleanOperator
{
	booleanOperatorAND = 0, booleanOperatorOR = 1, booleanOperatorSUB = 2
};

}

class cParamRender
{
public:

	//constructor with init
	cParamRender(const cParameterContainer *par, QVector<cObjectData> *objectData = NULL);

	int ambientOcclusionQuality; //ambient occlusion quality
	int auxLightNumber;
	int auxLightRandomNumber;
	int auxLightRandomSeed;
	int frameNo;
	int imageHeight; //image height
	int imageWidth; //image width
	int formulaMaterialId[NUMBER_OF_FRACTALS];
	int minN;	  // minimum number of iterations
	int N;
	int reflectionsMax;
	int repeatFrom;
	int DOFNumberOfPasses;

#ifdef CLSUPPORT
	int OpenCLDOFMethod;
	int OpenCLEngine;
	int OpenCLPixelsPerJob;
#endif

	params::enumPerspectiveType perspectiveType;
	params::enumAOMode ambientOcclusionMode;
	params::enumTextureMapType texturedBackgroundMapType;
	params::enumBooleanOperator booleanOperator[NUMBER_OF_FRACTALS - 1];
	fractal::enumDEMethod delta_DE_method;
	fractal::enumDEFunctionType delta_DE_function;

	bool ambientOcclusionEnabled; //enable global illumination
	bool auxLightPreEnabled[4];
	bool auxLightRandomEnabled;
	bool booleanOperatorsEnabled;
	bool constantDEThreshold;
	bool DOFEnabled;
	bool DOFHDRmode;
	bool envMappingEnable;
	bool fakeLightsEnabled;
	bool fogEnabled;
	bool glowEnabled;
	bool hybridFractalEnable;
	bool interiorMode;
	bool iterFogEnabled;
	bool iterThreshMode;	 //maxiter threshold mode
	bool legacyCoordinateSystem;
	bool limitsEnabled; // enable limits (intersections)
	bool mainLightEnable;
	bool mainLightPositionAsRelative;
	bool penetratingLights;
	bool raytracedReflections;
	bool shadow; //enable shadows
	bool slowShading; //enable fake gradient calculation for shading
	bool SSAO_random_mode;
	bool texturedBackground; //enable testured background
	bool useDefaultBailout;
	bool volumetricLightEnabled[5];
	bool volumetricLightAnyEnabled;
	bool volFogEnabled;

#ifdef CLSUPPORT
	bool useCustomOCLFormula;
#endif

	sRGB auxLightPreColour[4];
	sRGB background_color1; //background colour
	sRGB background_color2;
	sRGB background_color3;
	sRGB fogColor;
	sRGB glowColor1;
	sRGB glowColor2;
	sRGB iterFogColour1;
	sRGB iterFogColour2;
	sRGB iterFogColour3;
	sRGB mainLightColour;
	sRGB volFogColour1;
	sRGB volFogColour2;
	sRGB volFogColour3;

	double ambientOcclusion;
	double ambientOcclusionFastTune;
	double auxLightPreIntensity[4];
	double auxLightVisibility;
	double auxLightVisibilitySize;
	double auxLightRandomRadius;
	double auxLightRandomMaxDistanceFromFractal;
	double auxLightRandomIntensity;
	double background_brightness;
	double cameraDistanceToTarget; //zoom
	double constantFactor;
	double DEFactor; //factor for distance estimation steps
	double detailLevel; //DE threshold factor
	double DEThresh;
	double DOFFocus;
	double DOFRadius;
	double DOFBlurOpacity;
	double fakeLightsIntensity;
	double fakeLightsVisibility;
	double fakeLightsVisibilitySize;
	double fogVisibility;
	double formulaScale[NUMBER_OF_FRACTALS];
	double fov; //perspective factor
	double glowIntensity;
	double iterFogColor1Maxiter;
	double iterFogColor2Maxiter;
	double iterFogOpacity;
	double iterFogOpacityTrim;
	double mainLightAlpha;
	double mainLightBeta;
	double mainLightIntensity;
	double mainLightVisibility;
	double mainLightVisibilitySize;
	double resolution; //resolution of image in fractal coordinates
	double shadowConeAngle;
	double smoothness;
	double sweetSpotHAngle;
	double sweetSpotVAngle;
	double viewDistanceMax;
	double viewDistanceMin;
	double volFogColour1Distance;
	double volFogColour2Distance;
	double volFogDensity;
	double volFogDistanceFactor;
	double volumetricLightIntensity[5];

	sImageAdjustments imageAdjustments;

	CVector3 auxLightPre[4];
	CVector3 auxLightRandomCenter;
	CVector3 formulaPosition[NUMBER_OF_FRACTALS];
	CVector3 formulaRotation[NUMBER_OF_FRACTALS];
	CVector3 formulaRepeat[NUMBER_OF_FRACTALS];
	CVector3 limitMin;
	CVector3 limitMax;
	CVector3 repeat;
	CVector3 target;
	CVector3 camera; //view point
	CVector3 viewAngle;
	CVector3 topVector;

	CRotationMatrix mRotFormulaRotation[NUMBER_OF_FRACTALS];

	cPrimitives primitives;

	sCommonParams common;
};



#endif /* FRACTPARAMS_HPP_ */
