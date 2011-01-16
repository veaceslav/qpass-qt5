/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "export-dialog.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QFileDialog>

ExportDialog::ExportDialog(QString *path)
{
	this->path = path;
	setWindowTitle( tr("QPass - database export") );
	QFormLayout *layout = new QFormLayout(this);
	setLayout(layout);
	QHBoxLayout *pathLayout = new QHBoxLayout();
	pathEdit = new QLineEdit();
	QPushButton *browseButton = new QPushButton("...", this);
	pathLayout->addWidget(pathEdit);
	pathLayout->addWidget(browseButton);
	layout->addRow( tr("Path:"), pathLayout);
	QPushButton *exportButton = new QPushButton( tr("Export"), this);
	layout->addRow( QString::null, exportButton);
	connect(browseButton, SIGNAL(clicked()), this, SLOT(browse()));
	connect(exportButton, SIGNAL(clicked()), this, SLOT(exportSlot()));
}

void ExportDialog::browse()
{
	QFileDialog dialog;
	pathEdit->setText(dialog.getSaveFileName(this, tr("Database export file"), "", tr("QPass database files (*.qpa);;All Files (*.*)")));
}

void ExportDialog::exportSlot()
{
	*path = pathEdit->text();
	accept();
}

#include "export-dialog.moc"