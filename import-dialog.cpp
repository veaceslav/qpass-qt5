/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "import-dialog.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QFileDialog>


ImportDialog::ImportDialog(QString *path, QString *password, bool *method)
{
	this->path = path;
	this->password = password;
	this->method = method;
	setWindowTitle( tr("QPass - database import") );
	QFormLayout *layout = new QFormLayout(this);
	setLayout(layout);
	QHBoxLayout *pathLayout = new QHBoxLayout();
	pathEdit = new QLineEdit();
	QPushButton *browseButton = new QPushButton("...", this);
	pathLayout->addWidget(pathEdit);
	pathLayout->addWidget(browseButton);
	layout->addRow( tr("Path:"), pathLayout);
	passwordEdit = new QLineEdit(this);
	passwordEdit->setEchoMode(QLineEdit::Password);
	layout->addRow( tr("Password:"), passwordEdit);
	if(method != NULL)
	{
		appendRadio = new QRadioButton(tr("Append to existing entries"), this);
		appendRadio->setChecked(true);
		layout->addRow( tr("Method:"), appendRadio);
		QRadioButton *replaceRadio = new QRadioButton(tr("Replace entries"), this);
		layout->addRow( "", replaceRadio);
	}
	QPushButton *importButton = new QPushButton( tr("Import"), this);
	layout->addRow( QString::null, importButton);
	connect(browseButton, SIGNAL(clicked()), this, SLOT(browse()));
	connect(importButton, SIGNAL(clicked()), this, SLOT(importSlot()));
	
	pathEdit->setText(*path);
	passwordEdit->setText(*password);
}

void ImportDialog::browse()
{
	QFileDialog dialog;
	pathEdit->setText(dialog.getOpenFileName(this, tr("Database import file"), "", tr("QPass database files (*.qpa);;All Files (*.*)")));
}

void ImportDialog::importSlot()
{
	*path = pathEdit->text();
	*password = passwordEdit->text();
	if(method != NULL)
		*method = appendRadio->isChecked();
	accept();
}

#include "import-dialog.moc"