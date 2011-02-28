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

#include "PreviousPasswordDialog.h"
#include "DataModel.h"
#include "PredefinedSettings.h"

PreviousPasswordDialog::PreviousPasswordDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(checkData()));
	connect(openOtherButton, SIGNAL(clicked()), this, SLOT(acceptOtherDatabase()));
}

QString PreviousPasswordDialog::getPassword()
{
	return passwordEdit->text();
}

void PreviousPasswordDialog::checkData()
{
	int res = DataModel::checkDatabase(PredefinedSettings::databasePath(), passwordEdit->text());
	if(res == -1)
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("Incorrect password - QPass") );
		box.setText( tr("Incorrect password.") );
		box.setIcon(QMessageBox::Warning);
		box.exec();
		return;
	}
	else if(res == -2)
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("QPass") );
		box.setText( tr("Error opening database.") );
		box.setIcon(QMessageBox::Critical);
		box.exec();
		done(QDialog::Rejected);
	}
	else
		done(QDialog::Accepted);
}

void PreviousPasswordDialog::acceptOtherDatabase()
{
	done(2);
}
