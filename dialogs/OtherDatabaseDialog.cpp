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

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

#include "OtherDatabaseDialog.h"
#include "DataModel.h"

OtherDatabaseDialog::OtherDatabaseDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	passwordEdit2->setVisible(false);
	retypePasswordLabel->setVisible(false);

	connect(browseButton, SIGNAL(clicked()), this, SLOT(browse()));
}

QString OtherDatabaseDialog::getPassword()
{
	return passwordEdit1->text();
}

QString OtherDatabaseDialog::getPath()
{
	return pathEdit->text();
}

void OtherDatabaseDialog::setPath(const QString &path)
{
	pathEdit->setText(path);
}

bool OtherDatabaseDialog::isSetAsDefault()
{
	return setAsDefaultBox->isChecked();
}

void OtherDatabaseDialog::setAsDefault(bool isDefault)
{
	setAsDefaultBox->setChecked(isDefault);
}

int OtherDatabaseDialog::getMode()
{
	if(openExistingButton->isChecked())
		return OpenExisting;
	else
		return CreateNew;
}

void OtherDatabaseDialog::setMode(int mode) 
{
	if(mode == OpenExisting)
		openExistingButton->setChecked(true);
	else
		createNewButton->setChecked(true);
}

void OtherDatabaseDialog::browse()
{
	QString path;

	if(openExistingButton->isChecked())
	{
		path = QFileDialog::getOpenFileName(this, tr("Select database"), NULL, tr("QPass database files (*.qpa);;All Files (*)"));
	}
	else
	{
		path = QFileDialog::getSaveFileName(this, tr("Select database"), NULL, tr("QPass database files (*.qpa);;All Files (*)"));
	}

	pathEdit->setText(path);
}

void OtherDatabaseDialog::accept()
{
	if(openExistingButton->isChecked())
	{
		int ret = DataModel::checkDatabase(pathEdit->text(), passwordEdit1->text());
		if(ret == -1)
		{
			QMessageBox box(this);
			box.setWindowTitle("QPass");
			box.setText( tr("Password is incorrect") );
			box.setIcon( QMessageBox::Warning );
			box.exec();
			return;
		}
		else if(ret == -2)
		{
			QMessageBox box(this);
			box.setWindowTitle("QPass");
			box.setText( tr("Error reading database or database doesn't exist") );
			box.setIcon( QMessageBox::Critical );
			box.exec();
			return;
		}
	}
	else
	{
		if(passwordEdit1->text() != passwordEdit2->text())
		{
			QMessageBox box(this);
			box.setWindowTitle("QPass");
			box.setText( tr("Passwords doesn't match") );
			box.setIcon( QMessageBox::Warning );
			box.exec();
			return;
		}
		QFile file(pathEdit->text());
		if(!file.open(QIODevice::ReadWrite))
		{
			QMessageBox box(this);
			box.setWindowTitle("QPass");
			box.setText( tr("Selected file is not writable") );
			box.setIcon( QMessageBox::Warning );
			box.exec();
			return;
		}
		file.close();
		file.remove();
	}
	done(QDialog::Accepted);
}
