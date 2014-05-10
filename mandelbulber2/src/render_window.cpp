/*
 * render_window.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: krzysztof
 */


#include "render_window.hpp"
#include "interface.hpp"
#include "fractal_list.hpp"
#include "system.hpp"
#include "four_fractals.hpp"
#include "fractparams.hpp"
#include "render_job.hpp"

#include <QtGui>
#include <QtUiTools/QtUiTools>
#include <QColorDialog>


#define _USE_MATH_DEFINES
#include <math.h>

RenderWindow::RenderWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RenderWindow)
{
    ui->setupUi(this);
  	fractalWidgets = new QWidget*[4];
}

RenderWindow::~RenderWindow()
{
    delete ui;
}


void RenderWindow::testSlot(void)
{
	using namespace std;
	qDebug() << "Object name from slot" << this->sender()->objectName() << endl;

	printf("Hello World!\n");

	mainInterface->SynchronizeInterface(gPar, gParFractal, cInterface::read);

	cRenderJob *renderJob = new cRenderJob(gPar, gParFractal, mainInterface->mainImage, mainInterface->renderedImage);
	renderJob->Init(cRenderJob::still);
	renderJob->Execute();

	delete renderJob;
}

void RenderWindow::load(void)
{
	printf("load\n");
}

void RenderWindow::slotSliderMoved(int value)
{
	using namespace std;
	QString sliderName = this->sender()->objectName();
	QString type, parameterName;
	mainInterface->GetNameAndType(sliderName, &parameterName, &type);
	QString spinBoxName = QString("spinbox_") + parameterName;

	QDoubleSpinBox *spinBox = this->sender()->parent()->findChild<QDoubleSpinBox*>(spinBoxName);
	if(spinBox)
	{
		spinBox->setValue(value/100.0);
	}
	else
	{
		qWarning() << "slotSliderMoved() error: spinbox " << spinBoxName << " doesn't exists" << endl;
	}
}

void RenderWindow::slotDoubleSpinBoxChanged(double value)
{
	using namespace std;
	QString spinBoxName = this->sender()->objectName();
	QString type, parameterName;
	mainInterface->GetNameAndType(spinBoxName, &parameterName, &type);
	QString sliderName = QString("slider_") + parameterName;

	QSlider *slider = this->sender()->parent()->findChild<QSlider*>(sliderName);
	if (slider)
	{
		slider->setValue(value * 100.0);
	}
	else
	{
		qWarning() << "slotDoubleSpinBoxChanged() error: slider " << sliderName << " doesn't exists" << endl;
	}
}

void RenderWindow::slotIntSpinBoxChanged(int value)
{
	using namespace std;
	QString spinBoxName = this->sender()->objectName();
	QString type, parameterName;
	mainInterface->GetNameAndType(spinBoxName, &parameterName, &type);
	QString sliderName = QString("sliderInt_") + parameterName;

	QSlider *slider = this->sender()->parent()->findChild<QSlider*>(sliderName);
	if (slider)
	{
		slider->setValue(value);
	}
	else
	{
		qWarning() << "slotIntSpinBoxChanged() error: slider " << sliderName << " doesn't exists" << endl;
	}
}

void RenderWindow::slotLogSliderMoved(int value)
{
	using namespace std;
	QString sliderName = this->sender()->objectName();
	QString type, parameterName;
	mainInterface->GetNameAndType(sliderName, &parameterName, &type);
	QString lineEditName = QString("logedit_") + parameterName;

	QLineEdit *lineEdit = this->sender()->parent()->findChild<QLineEdit*>(lineEditName);
	if(lineEdit)
	{
		double dValue = pow(10.0, value/100.0);
		QString text = QString::number(dValue);
		lineEdit->setText(text);
	}
	else
	{
		qWarning() << "slotLogSliderMoved() error: lineEdit " << lineEditName << " doesn't exists" << endl;
	}
}

void RenderWindow::slotIntSliderMoved(int value)
{
	using namespace std;
	QString sliderName = this->sender()->objectName();
	QString type, parameterName;
	mainInterface->GetNameAndType(sliderName, &parameterName, &type);
	QString spinboxName = QString("spinboxInt_") + parameterName;

	QSpinBox *spinbox = this->sender()->parent()->findChild<QSpinBox*>(spinboxName);
	if(spinbox)
	{
		spinbox->setValue(value);
	}
	else
	{
		qWarning() << "slotLogSliderMoved() error: lineEdit " << spinboxName << " doesn't exists" << endl;
	}
}

void RenderWindow::slotLogLineEditChanged(const QString &text)
{
	using namespace std;
	QString lineEditName = this->sender()->objectName();
	QString type, parameterName;
	mainInterface->GetNameAndType(lineEditName, &parameterName, &type);
	QString sliderName = QString("logslider_") + parameterName;

	QSlider *slider = this->sender()->parent()->findChild<QSlider*>(sliderName);
	if (slider)
	{
		double value = text.toDouble();
		if(value > 0.0)
		{
			int sliderPosition = log10(text.toDouble()) * 100.0;
			slider->setValue(sliderPosition);
		}
		else
		{
			qWarning() << "slotLogLineEditChanged() error: value from " << lineEditName << " is not greater zero" << endl;
		}
	}
	else
	{
		qWarning() << "slotLogLineEditChanged() error: slider " << sliderName << " doesn't exists" << endl;
	}
}

void RenderWindow::slotPresedOnColorButton(void)
{
	QPushButton *pushButton = qobject_cast<QPushButton*>(this->sender());
	QString pushButtonName = pushButton->objectName();
	QColorDialog colorDialog(mainInterface->mainWindow);
	QColor color;
	color.setRed(pushButton->property("selectedColor_r").toInt() / 256);
	color.setGreen(pushButton->property("selectedColor_g").toInt() / 256);
	color.setBlue(pushButton->property("selectedColor_b").toInt() / 256);
	colorDialog.setCurrentColor(color);
	colorDialog.exec();
	color = colorDialog.currentColor();
	MakeIconForButton(color, pushButton);
	pushButton->setProperty("selectedColor_r", color.red() * 256);
	pushButton->setProperty("selectedColor_g", color.green() * 256);
	pushButton->setProperty("selectedColor_b", color.blue() * 256);
}

void RenderWindow::slotMenuSaveDocksPositions()
{
	settings.setValue("mainWindowGeometry", saveGeometry());
	settings.setValue("mainWindowState", saveState());
	qDebug() << "settings saved";
}

void RenderWindow::slotChangedFractalCombo(int index)
{
	QString comboName = this->sender()->objectName();
	int fractalNumber = comboName.right(1).toInt() - 1;

	if(fractalList[index].internalID > 0)
	{
		QString formulaName = fractalList[index].internalNane;
		QString uiFilename = systemData.sharedDir + QDir::separator() + "qt" + QDir::separator() + "fractal_" + formulaName + ".ui";

		if(fractalWidgets[fractalNumber]) delete fractalWidgets[fractalNumber];
		fractalWidgets[fractalNumber] = NULL;

		QUiLoader loader;
		QFile uiFile(uiFilename);

		if(uiFile.exists())
		{
			uiFile.open(QFile::ReadOnly);
			fractalWidgets[fractalNumber] = loader.load(&uiFile);
			QVBoxLayout *layout = ui->dockWidget_fractal->findChild<QVBoxLayout*>("verticalLayout_fractal_" + QString::number(fractalNumber + 1));
			layout->addWidget(fractalWidgets[fractalNumber]);
			uiFile.close();
			fractalWidgets[fractalNumber]->show();
			mainInterface->ConnectSignalsForSlidersInWindow(fractalWidgets[fractalNumber]);
			mainInterface->SynchronizeInterfaceWindow(fractalWidgets[fractalNumber], &gParFractal[fractalNumber], cInterface::write);
		}
		else
		{
			qCritical() << "Can't open file " << uiFilename << " Fractal ui file can't be loaded";
		}
	}
	else
	{
		if(fractalWidgets[fractalNumber]) delete fractalWidgets[fractalNumber];
		fractalWidgets[fractalNumber] = NULL;
	}

}

void RenderWindow::slotImageScrolledAreaResized(int width, int height)
{
	int selectedScale = ui->comboBox_imageScale->currentIndex();

	if (selectedScale == 0)
	{
		double scale1 = (double)height / mainInterface->mainImage->GetHeight();
		double scale2 = (double)width / mainInterface->mainImage->GetWidth();
		double scale = min(scale1, scale2);
		mainInterface->mainImage->CreatePreview(scale, mainInterface->renderedImage);
		mainInterface->mainImage->UpdatePreview();
		mainInterface->renderedImage->setMinimumSize(mainInterface->mainImage->GetPreviewWidth(), mainInterface->mainImage->GetPreviewHeight());
	}

}

void RenderWindow::slotChangedImageScale(int index)
{
	double scale = ImageScaleComboSelection2Double(index);
	mainInterface->mainImage->CreatePreview(scale, mainInterface->renderedImage);
	mainInterface->mainImage->UpdatePreview();
	mainInterface->renderedImage->setMinimumSize(mainInterface->mainImage->GetPreviewWidth(), mainInterface->mainImage->GetPreviewHeight());
}

double RenderWindow::ImageScaleComboSelection2Double(int index)
{
	double scales[] = {0.0, 4.0, 2.0, 1.0, 0.5, 0.25, 0.1};
	if(index < 7)
	{
		return scales[index];
	}
	else
	{
		qCritical() << "Wrong image scale";
		return -1.0;
	}
}

//=================== rendered image widget ==================/

RenderedImage::RenderedImage(QWidget *parent)
    : QWidget(parent)
{ }

void RenderedImage::paintEvent(QPaintEvent *event)
{
  mainInterface->mainImage->RedrawInWidget();
}
