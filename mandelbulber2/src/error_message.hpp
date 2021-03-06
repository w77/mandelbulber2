/**
 * Mandelbulber v2, a 3D fractal generator
 *
 * cErrorMessage class - displaying warning/error messages as popup window and stderr output
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

#ifndef ERROR_MESSAGE_HPP_
#define ERROR_MESSAGE_HPP_

#include <QtCore>
#include "system.hpp"

class cErrorMessage: public QObject
{
Q_OBJECT
public:
	cErrorMessage(QObject *parent = NULL) :
			QObject(parent)
	{
	}

	enum enumMessageType
	{
		warningMessage, errorMessage, infoMessage
	};

	static void showMessage(QString text, enumMessageType messageType, QWidget *parent = NULL);

public slots:
	void slotShowMessage(QString text, cErrorMessage::enumMessageType messageType, QWidget *parent =
			NULL);
};

extern cErrorMessage *gErrorMessage;

#endif /* ERROR_MESSAGE_HPP_ */

