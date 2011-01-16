/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>

class ImportDialog : public QDialog
{
	Q_OBJECT
	private:
		QLineEdit *pathEdit;
		QLineEdit *passwordEdit;
		QRadioButton *appendRadio;
		QString *path;
		QString *password;
		bool *method;
	public:
		ImportDialog(QString *path, QString *password, bool *method = NULL);
	public slots:
		void browse();
		void importSlot();
};

#endif //EXPORT_DIALOG_H