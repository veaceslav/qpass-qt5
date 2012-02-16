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

#include "PreviousPasswordDialog.h"
#include "DataModel.h"
#include "DataAccess.h"
#include "PredefinedSettings.h"

PreviousPasswordDialog::PreviousPasswordDialog(QString &databasePath, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	databaseLocationLabel->setText(databasePath);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(checkData()));
	connect(openOtherButton, SIGNAL(clicked()), this, SLOT(acceptOtherDatabase()));
	this->databasePath = databasePath;
}

QString PreviousPasswordDialog::getPassword()
{
	return passwordEdit->text();
}

void PreviousPasswordDialog::checkData()
{
	int res = DataModel::checkDatabase(databasePath, passwordEdit->text());
	if(res == INVALID_PASSWORD)
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("Incorrect password - QPass") );
		box.setText( tr("Incorrect password.") );
		box.setIcon(QMessageBox::Warning);
		box.exec();
		return;
	}
	else if(res == FILE_ERROR)
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("QPass") );
		box.setText( tr("Error opening database.") );
		box.setIcon(QMessageBox::Critical);
		box.exec();
		done(QDialog::Rejected);
	}
	else if(res == 0)
		done(QDialog::Accepted);
}

void PreviousPasswordDialog::acceptOtherDatabase()
{
	done(2);
}
