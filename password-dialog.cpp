/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "password-dialog.h"

#include <QLineEdit>
#include <QFormLayout>
#include <QHBoxLayout>

PasswordDialog::PasswordDialog(QString *buffer, bool n)
{
	this->n = n;
	this->buffer = buffer;
	setWindowTitle( tr("QPass - database password") );
	QVBoxLayout *layout = new QVBoxLayout();
	setLayout(layout);
	QWidget *form = new QWidget();
	QFormLayout *formLayout = new QFormLayout(form);
	edit = new QLineEdit();
	edit->setEchoMode(QLineEdit::Password);
	if(n)
	{
		formLayout->addRow( tr("Password to new database:"), edit);
		setWindowTitle( tr("QPass - database creation") );
		edit2 = new QLineEdit();
		edit2->setEchoMode(QLineEdit::Password);
		formLayout->addRow( tr("Retype password:"), edit2);
	}
	else
	{
		formLayout->addRow( tr("Password:"), edit);
	}
	QWidget *buttons = new QWidget();
	okButton = new QPushButton( tr("OK") );
	okButton->setDefault(true);
	QHBoxLayout *buttonsLayout = new QHBoxLayout(buttons);
	QWidget *space = new QWidget();
	space->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	buttonsLayout->addWidget(space);
	buttonsLayout->addWidget(okButton);
	if(n)
	{
			importButton = new QPushButton( tr("Import existing database") );
			buttonsLayout->addWidget(importButton);
			connect(importButton, SIGNAL(clicked()), this, SLOT(returnImport()));
	}
	QWidget *space2 = new QWidget();
	space2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	buttonsLayout->addWidget(space2);
	layout->addWidget(form);
	layout->addWidget(buttons);
	connect(okButton, SIGNAL(clicked()), this, SLOT(returnPassword()));
}

void PasswordDialog::returnPassword()
{
	if(n)
	{
		if(edit->text() == edit2->text())
		{
			*buffer = edit->text();
			accept();
		}
		else 
			done(2);
	}
	else 
	{
		*buffer = edit->text();
		accept();
	}
}

void PasswordDialog::returnImport()
{
	done(3);
}
#include "password-dialog.moc"