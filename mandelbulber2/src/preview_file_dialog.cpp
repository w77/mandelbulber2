/**
 * Mandelbulber v2, a 3D fractal generator
 *
 * PreviewFileDialog class - extension of QFileDialog class. Added preview display
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

#include "preview_file_dialog.h"
#include <QGridLayout>
#include "interface.hpp"
#include "queue.hpp"
#include "settings.hpp"
#include "initparameters.hpp"

PreviewFileDialog::PreviewFileDialog(QWidget *parent) :
		QFileDialog(parent)
{
	setOption(QFileDialog::DontUseNativeDialog);

	preview = NULL;
	vboxlayout = new QVBoxLayout();

	checkbox = new QCheckBox(tr("Preview"));
	checkbox->setChecked(true);

	preview = new QLabel("", this);
	preview->setAlignment(Qt::AlignCenter);
	preview->setObjectName("label_preview");

	thumbWidget = new cThumbnailWidget(200, 200, 1, this);

	info = new QLabel("");

	progressBar = new QProgressBar;
	progressBar->setMaximum(1000);
	progressBar->setAlignment(Qt::AlignCenter);
	progressBar->hide();

	presetAddButton = new QPushButton;
	presetAddButton->setText(tr("Add to presets"));

	queueAddButton = new QPushButton;
	queueAddButton->setText(tr("Add to queue"));

	vboxlayout->addWidget(checkbox);
	vboxlayout->addWidget(preview);
	vboxlayout->addWidget(thumbWidget);
	vboxlayout->addWidget(progressBar);
	vboxlayout->addWidget(info);
	vboxlayout->addWidget(presetAddButton);
	vboxlayout->addWidget(queueAddButton);

	thumbWidget->show();
	vboxlayout->addStretch();

	//add to existing layout
	QGridLayout *gridlayout = (QGridLayout*) this->layout();
	gridlayout->addLayout(vboxlayout, 1, 3, 3, 1);

	connect(this,
					SIGNAL(currentChanged(const QString&)),
					this,
					SLOT(OnCurrentChanged(const QString&)));
	connect(presetAddButton, SIGNAL(clicked()), this, SLOT(OnPresetAdd()));
	connect(queueAddButton, SIGNAL(clicked()), this, SLOT(OnQueueAdd()));
	connect(thumbWidget, SIGNAL(thumbnailRendered()), this, SLOT(slotHideProgressBar()));
	connect(thumbWidget,
					SIGNAL(updateProgressAndStatus(const QString&, const QString&, double)),
					this,
					SLOT(slotUpdateProgressAndStatus(const QString&, const QString&, double)));
}

PreviewFileDialog::~PreviewFileDialog()
{
	delete vboxlayout;
	delete preview;
	delete info;
	delete progressBar;
	delete presetAddButton;
	delete queueAddButton;
}

void PreviewFileDialog::OnPresetAdd()
{
	fcopy(filename, systemData.dataDirectory + "toolbar/" + QFileInfo(filename).fileName());
	gMainInterface->mainWindow->slotPopulateToolbar();
}

void PreviewFileDialog::OnQueueAdd()
{
	gQueue->Append(filename, cQueue::queue_STILL);
}

void PreviewFileDialog::OnCurrentChanged(const QString & _filename)
{
	filename = _filename;
	QPixmap pixmap;
	if (QFileInfo(filename).suffix() == QString("fract") && checkbox->isChecked())
	{
		thumbWidget->show();
		preview->hide();
		cSettings parSettings(cSettings::formatFullText);
		parSettings.BeQuiet(true);
		if (parSettings.LoadFromFile(filename))
		{
			progressBar->show();
			cParameterContainer *par = new cParameterContainer;
			cFractalContainer *parFractal = new cFractalContainer;
			InitParams(par);
			for (int i = 0; i < NUMBER_OF_FRACTALS; i++)
				InitFractalParams(&parFractal->at(i));

			/****************** TEMPORARY CODE FOR MATERIALS *******************/

			InitMaterialParams(1, par);

			/*******************************************************************/

			if (parSettings.Decode(par, parFractal))
			{
				thumbWidget->AssignParameters(*par, *parFractal);
				thumbWidget->update();
			}
			else
			{
				preview->setText(" ");
				info->setText(" ");
			}
			delete par;
			delete parFractal;
		}
	}
	else
	{
		thumbWidget->hide();
		preview->show();
		pixmap.load(filename);
		if (pixmap.isNull() || !checkbox->isChecked())
		{
			preview->setText(" ");
			info->setText(" ");
		}
		else
		{
			preview->setPixmap(pixmap.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
			QString text = QString::number(pixmap.width()) + QString(" x ")
					+ QString::number(pixmap.height());
			info->setText(text);
		}
	}
}

void PreviewFileDialog::slotUpdateProgressAndStatus(const QString &text,
		const QString &progressText, double progress)
{
	info->setText(text);
	if (!progressBar->isVisible()) progressBar->setVisible(true);
	progressBar->setValue(progress * 1000.0);
	progressBar->setTextVisible(true);
	progressBar->setFormat(progressText);
}

void PreviewFileDialog::slotHideProgressBar()
{
	progressBar->hide();
}
