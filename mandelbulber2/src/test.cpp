/**
 * Mandelbulber v2, a 3D fractal generator
 *
 * class for testing mandelbulber functionality on the cli
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

#include "test.hpp"
#include "system.hpp"
#include "parameters.hpp"
#include "fractal_container.hpp"
#include "animation_frames.hpp"
#include "keyframes.hpp"
#include "initparameters.hpp"
#include "settings.hpp"
#include "cimage.hpp"
#include "render_job.hpp"
#include "netrender.hpp"

void Test::renderExamples()
{
	// this renders all example files in a resolution of 5x5 px
	// and benchmarks the runtime

	QString examplePath = QDir::toNativeSeparators(systemData.sharedDir + QDir::separator() + "examples");
	QDirIterator it(examplePath, QStringList() << "*.fract", QDir::Files);

	cParameterContainer* testPar = new cParameterContainer;
	cFractalContainer* testParFractal = new cFractalContainer;
	cAnimationFrames* testAnimFrames = new cAnimationFrames;
	cKeyframes* testKeyframes = new cKeyframes;

	testPar->SetContainerName("main");
	InitParams(testPar);
	/****************** TEMPORARY CODE FOR MATERIALS *******************/

	InitMaterialParams(1, testPar);

	/*******************************************************************/
	for (int i = 0; i < NUMBER_OF_FRACTALS; i++)
	{
		testParFractal->at(i).SetContainerName(QString("fractal") + QString::number(i));
		InitFractalParams(&testParFractal->at(i));
	}
	bool stopRequest = false;
	cImage *image = new cImage(testPar->Get<int>("image_width"), testPar->Get<int>("image_height"));
	cRenderingConfiguration config;
	config.DisableRefresh();
	config.DisableProgressiveRender();

	while (it.hasNext())
	{
		QString filename = it.next();
		cSettings parSettings(cSettings::formatFullText);
		parSettings.BeQuiet(true);
		parSettings.LoadFromFile(filename);
		parSettings.Decode(testPar, testParFractal, testAnimFrames, testKeyframes);
		testPar->Set("image_width", 5);
		testPar->Set("image_height", 5);
		cRenderJob *renderJob = new cRenderJob(testPar, testParFractal, image, &stopRequest);
		renderJob->Init(cRenderJob::still, config);
		QVERIFY2(renderJob->Execute(), "execution failed.");
		delete renderJob;	
	}

	delete image;
	delete testKeyframes;
	delete testAnimFrames;
	delete testParFractal;
	delete testPar;
}

void Test::netrender()
{
	// test connection of server / client over localhost
	CNetRender* netRenderServer = new CNetRender(1);
	CNetRender* netRenderClient = new CNetRender(1);
	netRenderServer->SetServer(5555);
	netRenderClient->SetClient("127.0.0.1", 5555);

	QTest::qWait(100);

	CNetRender::netRenderStatus clientStatus = netRenderClient->GetStatus();
	QVERIFY2(clientStatus == CNetRender::netRender_READY,
		QString("client status wrong: should be 'READY' but is '%1'.")
			.arg(CNetRender::GetStatusText(clientStatus)).toStdString().c_str()
	);

	QVERIFY2(netRenderServer->GetClientCount() == 1,
		QString("client not connected to server.").toStdString().c_str()
	);

	delete netRenderClient;
	delete netRenderServer;
}
