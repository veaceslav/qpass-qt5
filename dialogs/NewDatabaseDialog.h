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

#ifndef NEWDATABASEDIALOG_H
#define NEWDATABASEDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QWidget>
#include <QStackedWidget>

#include "ui_NewDatabaseDialog.h"

/*!
 * Provides welcome dialog.
 */
class NewDatabaseDialog : public QDialog, private Ui::NewDatabaseDialog
{
	public:
		NewDatabaseDialog(QWidget *parent = 0);
};

#endif //NEWDATABASEDIALOG_H
