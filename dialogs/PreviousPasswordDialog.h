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


#ifndef PREVIOUSPASSWORDDIALOG_H
#define PREVIOUSPASSWORDDIALOG_H

#include <QDialog>

#include "ui_PreviousPasswordDialog.h"

/*!
 * Provides current password entry dialog.
 */
class PreviousPasswordDialog : public QDialog, private Ui::PreviousPasswordDialog
{
	Q_OBJECT
	public:
		PreviousPasswordDialog(QWidget *parent = 0);
		/*! 
		* Returns password provided by the user.
		*/
		QString getPassword();
	private slots:
		void accept();
};

#endif //PREVIOUSPASSWORDDIALOG_H
