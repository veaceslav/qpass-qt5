/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>

class PasswordDialog : public QDialog
{
	Q_OBJECT
	private:
		bool n;
		QPushButton *okButton;
		QPushButton *importButton;
		QString *buffer;
		QLineEdit *edit;
		QLineEdit *edit2;
	public:
		PasswordDialog(QString *buffer, bool n);
	private slots:
		void returnPassword();
		void returnImport();
};

#endif //PASSWORDDIALOG_H