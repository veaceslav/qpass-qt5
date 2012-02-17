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

#ifndef OTHERDATABASEDIALOG_H
#define OTHERDATABASEDIALOG_H

#include <QDialog>

#include "DataModel.h"

#include "ui_OtherDatabaseDialog.h"

class OtherDatabaseDialog : public QDialog, private Ui::OtherDatabaseDialog
{
	Q_OBJECT;

public:
	OtherDatabaseDialog(DataModel *model, QWidget *parent = 0);
	QString getPath();
	void setPath(const QString &path);
	bool isSetAsDefault();
	void setAsDefault(bool isDefault);
	void setMode(int mode);
	enum Modes
	{
		OpenExisting,
		CreateNew
	};

private:
	DataModel *model;

private slots:
	void browse();
	void accept();
};

#endif
