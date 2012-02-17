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

#include "DatabaseImportDialog.h"

#include <QFileDialog>
#include <QMessageBox>

#include "DataModel.h"

DatabaseImportDialog::DatabaseImportDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	connect(browseButton, SIGNAL(clicked()), this, SLOT(browse()));
}

QString DatabaseImportDialog::getPassword()
{
	return passwordEdit->text();
}

QString DatabaseImportDialog::getPath()
{
	return pathEdit->text();
}

int DatabaseImportDialog::getMode()
{
	if(appendButton->isChecked())
		return DatabaseImportDialog::Append;
	else
		return DatabaseImportDialog::Replace;
}

int DatabaseImportDialog::getFormat()
{
	if(qpaFormat->isChecked())
		return DataModel::Native;
	else
		return DataModel::Csv;
}

void DatabaseImportDialog::browse()
{
	QFileDialog dialog(this);
	if(qpaFormat->isChecked())
		dialog.setFilter( tr("QPass database files (*.qpa);;All Files (*)") );
	else if(csvFormat->isChecked())
		dialog.setFilter( tr("CSV files (*.csv);;All Files (*)") );
	dialog.setFileMode( QFileDialog::ExistingFile );
	if(dialog.exec() == QDialog::Accepted)
		pathEdit->setText( dialog.selectedFiles().at(0));
}

void DatabaseImportDialog::accept()
{
	if(pathEdit->text().isEmpty())
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("QPass") );
		box.setText( tr("File path can not be empty") );
		box.setIcon( QMessageBox::Warning );
		box.exec();
		return;
	}
	if(qpaFormat->isChecked())
	{
		errorCode err = DataModel::checkDatabase( getPath(), getPassword());
		if(err == INVALID_PASSWORD)
		{
			QMessageBox box(this);
			box.setWindowTitle( tr("QPass") );
			box.setText( tr("Incorrect password.") );
			box.setIcon(QMessageBox::Warning);
			box.exec();
			return;
		}
		else if(err == FILE_ERROR)
		{
			QMessageBox box(this);
			box.setWindowTitle( tr("QPass") );
			box.setText( tr("Selected database is corrupted or it is not a qpass database.") );
			box.setIcon(QMessageBox::Warning);
			box.exec();
			return;
		}
	}
	done(QDialog::Accepted);
}
