/***************************************************************************
 *   Copyright (c) 2010-2012 Mateusz Piękos <mateuszpiekos@gmail.com>      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef COLUMNORGANIZATIONDIALOG_H
#define COLUMNORGANIZATIONDIALOG_H

#include <QDialog>
#include <QString>

#include "ui_ColumnOrganizationDialog.h"
#include "DataModel.h"

class ColumnOrganizationDialog : public QDialog, private Ui::ColumnOrganizationDialog
{
	Q_OBJECT;
public:

	ColumnOrganizationDialog(QWidget *parent = 0);
	QVector<DataModel::Columns> getColumnOrganization() const;


private slots:
	void accept();
};

#endif
