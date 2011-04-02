/***************************************************************************
 *   Copyright (c) 2010-2011 Mateusz Piękos <mateuszpiekos@gmail.com>      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef PASSWORDVIEWER_H
#define PASSWORDVIEWER_H

#include "ui_PasswordViewer.h"

#include <QDialog>

class PasswordViewer : public QDialog, private Ui::PasswordViewer
{
Q_OBJECT;
public:
	PasswordViewer(QWidget *parent, QString password);
};

#endif
