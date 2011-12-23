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

#ifndef OTHERDATABASEDIALOG_H
#define OTHERDATABASEDIALOG_H

#include <QDialog>

#include "ui_OtherDatabaseDialog.h"

class OtherDatabaseDialog : public QDialog, private Ui::OtherDatabaseDialog
{
	Q_OBJECT;

public:
	OtherDatabaseDialog(QWidget *parent = 0);
	QString getPassword();
	QString getPath();
	void setPath(const QString &path);
	bool isSetAsDefault();
	void setAsDefault(bool isDefault);
	int getMode();
	void setMode(int mode);
	enum Modes
	{
		OpenExisting,
		CreateNew
	};
private slots:
	void browse();
	void accept();
};

#endif
