/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class ExportDialog : public QDialog
{
	Q_OBJECT
	private:
		QLineEdit *pathEdit;
		QString *path;
	public:
		ExportDialog(QString *path);
	public slots:
		void browse();
		void exportSlot();
};

#endif //EXPORT_DIALOG_H