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

#ifndef DATABASEIMPORTDIALOG_H
#define DATABASEIMPORTDIALOG_H

#include <QDialog>
#include <QString>

#include "ui_DatabaseImportDialog.h"

class DatabaseImportDialog : public QDialog, private Ui::DatabaseImportDialog
{
	Q_OBJECT
	public:
		DatabaseImportDialog(QWidget *parent);
		QString getPassword();
		QString getPath();
		int getMode();
		enum mode
		{
			Append,
			Replace
		};
	private slots:
		void browse();
		void accept();
};

#endif //DATABASEIMPORTDIALOG_H
