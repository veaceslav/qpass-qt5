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

#include "DatabaseExportDialog.h"

#include <QFileDialog>
#include <QMessageBox>

#include "DataModel.h"

DatabaseExportDialog::DatabaseExportDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	connect(browseButton, SIGNAL(clicked()), this, SLOT(browse()));
}

QString DatabaseExportDialog::getPassword()
{
	return passwordEdit1->text();
}

QString DatabaseExportDialog::getPath()
{
	return pathEdit->text();
}

int DatabaseExportDialog::getFormat()
{
	if(qpaFormat->isChecked())
		return DataModel::Native;
	else
		return DataModel::Csv;
}

void DatabaseExportDialog::browse()
{
	QFileDialog dialog(this);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	
	if(qpaFormat->isChecked())
	{
		dialog.setFilter("QPass database files (*.qpa);;All Files (*)");
		dialog.setDefaultSuffix("qpa");
	}
	else if(csvFormat->isChecked())
	{
		dialog.setFilter("CSV files (*.csv);;All Files (*)");
		dialog.setDefaultSuffix("csv");
	}
	if(dialog.exec() == QDialog::Accepted)
	{
		pathEdit->setText(dialog.selectedFiles()[0]);
	}
}

void DatabaseExportDialog::accept()
{
	if(passwordEdit1->text() != passwordEdit2->text())
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("QPass") );
		box.setText( tr("Passwords does not match") );
		box.setIcon( QMessageBox::Warning );
		box.exec();
		return;
	}
	if(pathEdit->text().isEmpty())
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("QPass") );
		box.setText( tr("File path can not be empty") );
		box.setIcon( QMessageBox::Warning );
		box.exec();
		return;
	}
	done(QDialog::Accepted);
}
