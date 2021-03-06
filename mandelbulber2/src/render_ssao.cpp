/**
 * Mandelbulber v2, a 3D fractal generator
 *
 * cRenderSSAO class - renders Screen Space Ambient Occlusion effect
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

#include "render_ssao.h"
#include "progress_text.hpp"
#include "ssao_worker.h"
#include "global_data.hpp"
#include "system.hpp"

cRenderSSAO::cRenderSSAO(const cParamRender *_params, const sRenderData *_renderData,
		cImage *_image) :
		QObject()
{
	params = _params;
	data = _renderData;
	image = _image;
	qualityFactor = 1.0;
	progressive = 0;
	numberOfThreads = data->configuration.GetNumberOfThreads();
}

cRenderSSAO::~cRenderSSAO()
{
	//nothing to destroy
}

void cRenderSSAO::RenderSSAO(QList<int> *list)
{
	WriteLog("cRenderSSAO::RenderSSAO()", 2);
	//prepare multiple threads
	QThread **thread = new QThread*[numberOfThreads];
	cSSAOWorker::sThreadData *threadData = new cSSAOWorker::sThreadData[numberOfThreads];
	cSSAOWorker **worker = new cSSAOWorker*[numberOfThreads];

	cProgressText progressText;
	progressText.ResetTimer();

	//create list of lines to render for each CPU core
	QList<int> *lists = NULL;
	if (list)
	{
		lists = new QList<int> [numberOfThreads];
		for (int i = 0; i < list->size(); i++)
		{
			int y = list->at(i);
			int mod = y % numberOfThreads;
			lists[mod].append(y);
		}
	}

	double qualityFactor;
	if (progressive > 0)
	{
		qualityFactor = sqrt(1.0 / (progressive * 2.0));
	}
	else
	{
		qualityFactor = 1.0;
	}
	int quality = params->ambientOcclusionQuality * params->ambientOcclusionQuality * qualityFactor;
	if (quality < 3) quality = 3;

	for (int i = 0; i < numberOfThreads; i++)
	{
		threadData[i].startLine = i;
		threadData[i].noOfThreads = numberOfThreads;
		threadData[i].quality = quality;
		threadData[i].done = 0;
		threadData[i].progressive = progressive;
		threadData[i].stopRequest = false;

		if (list) threadData[i].list = &lists[i];
		else threadData[i].list = NULL;
	}

	QString statusText;
	QString progressTxt;

	WriteLog("Start rendering SSAO", 2);

	for (int i = 0; i < numberOfThreads; i++)
	{
		WriteLog(QString("Thread ") + QString::number(i) + " create", 3);
		thread[i] = new QThread;
		worker[i] = new cSSAOWorker(params, &threadData[i], data, image); //Warning! not needed to delete object
		worker[i]->moveToThread(thread[i]);
		QObject::connect(thread[i], SIGNAL(started()), worker[i], SLOT(doWork()));
		QObject::connect(worker[i], SIGNAL(finished()), thread[i], SLOT(quit()));
		QObject::connect(worker[i], SIGNAL(finished()), worker[i], SLOT(deleteLater()));
		thread[i]->setObjectName("SSAOWorker #" + QString::number(i));
		thread[i]->start();
		WriteLog(QString("Thread ") + QString::number(i) + " started", 3);
	}

	int totalDone = 0;
	int toDo;
	if (list)
	{
		toDo = list->size();
	}
	else
	{
		toDo = image->GetHeight();
	}

	while (totalDone < toDo)
	{
		gApplication->processEvents();

		Wait(10);

		if (*data->stopRequest)
		{
			for (int i = 0; i < numberOfThreads; i++)
				threadData[i].stopRequest = true;
			if (!list)
			{
				//*data->stopRequest = false;
			}
			break;
		}

		totalDone = 0;
		for (int i = 0; i < numberOfThreads; i++)
		{
			totalDone += threadData[i].done;
		}

		double percentDone = (double) totalDone / toDo;
		statusText = QObject::tr("Rendering SSAO effect in progress");
		progressTxt = progressText.getText(percentDone);

		emit updateProgressAndStatus(statusText, progressTxt, percentDone);
	}

	for (int i = 0; i < numberOfThreads; i++)
	{
		while (thread[i]->isRunning())
		{
			gApplication->processEvents();
		};
		WriteLog(QString("Thread ") + QString::number(i) + " finished", 3);
		delete thread[i];
	}

	//status bar and progress bar
	double percentDone = 1.0;
	statusText = QObject::tr("Idle");
	progressTxt = progressText.getText(percentDone);

	emit updateProgressAndStatus(statusText, progressTxt, percentDone);

	delete[] thread;
	delete[] threadData;
	delete[] worker;
	if (list) delete[] lists;

	WriteLog("cRenderSSAO::RenderSSAO(): memory released", 2);

	WriteLog("Rendering SSAO finished", 2);
}
