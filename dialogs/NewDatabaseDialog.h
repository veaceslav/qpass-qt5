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

#include "ui_NewDatabaseDialogPage1.h"
#include "ui_NewDatabaseDialogPage2.h"

/*!
 * Provides dialog to prompt how to create new database.
 */
class NewDatabaseDialog : public QDialog//, private Ui::NewDatabaseDialog
{
	Q_OBJECT
	public:
		NewDatabaseDialog(QWidget *parent = 0);
		/*! Returns password to new database.
		* 
		* Returned password is ready to use becouse dialog
		* take care of user mistakes by askings for password twice
		* and checks if password is not empty.
		* 
		* @return password to new database.
		*/
		QString value();
	private:
		Ui::NewDatabaseDialogPage1 uiWelcome;
		Ui::NewDatabaseDialogPage2 uiPassword;
		QStackedWidget *stackedWidget;
		QWidget *welcomeWidget;
		QWidget *passwordWidget;
	private slots:
		void next();
		void accept();
};

#endif //NEWDATABASEDIALOG_H
