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

#ifndef DATABASEEXPORTDIALOG_H
#define DATABASEEXPORTDIALOG_H

#include <QDialog>
#include <QString>

#include "ui_DatabaseExportDialog.h"
#include "ColumnOrganizationDialog.h"

class DatabaseExportDialog : public QDialog, private Ui::DatabaseExportDialog
{
	Q_OBJECT;

	QVector<DataModel::Columns> organization;//Default configuration when empty
public:
	DatabaseExportDialog(QWidget *parent);
	QString getPassword();
	QString getPath();
	int getFormat();
	QVector<DataModel::Columns> getColumnOrganization() const;
private slots:
	void showColumnOrganizationDialog();
	void browse();
	void accept();
};

#endif //DATABASEEXPORTDIALOG_H
