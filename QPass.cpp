/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "QPass.h"
#include "database.h"
#include "password-dialog.h"
#include "about.h"
#include "export-dialog.h"
#include "import-dialog.h"

#include <QtGui/QLabel>
#include <QtGui/QMenuBar>
#include <QClipboard>
#include <QApplication>
#include <QMessageBox>
#include <QPixmap>
#include <QIcon>
#include <QSettings>
#include <QCloseEvent>

QPass::QPass()
{
	createGUI();
	if(readData() != 0)
		qApp->quit();
}

QPass::~QPass()
{
	trayIcon->hide();
	writeSettings();
	if(dataChanged)
		saveData();
}
int QPass::readData(QString password, QString dbpath)
{
	char error;
	QString temppassword;
	QList<QStringList> data;
	bool first = true;
	bool typed = false;
	if(!password.isNull())
	{
		temppassword = password;
		first = false;
		typed = true;
	}
	else
	{
		temppassword = key;
	}
	do
	{
		Database database;
		error = database.readData(&data, temppassword, dbpath);
		if(error == Database::badformat)
		{
			QMessageBox msgBox;
			msgBox.setWindowTitle("QPass");
			msgBox.setText( tr("Database file is corrupted") );
			msgBox.setStandardButtons(QMessageBox::Ok);
			msgBox.setDefaultButton(QMessageBox::Ok);
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.exec();
			exit(0);
		}
		else if(error == Database::errorreading)
		{
			QMessageBox msgBox;
			msgBox.setWindowTitle("QPass");
			msgBox.setText( tr("Error in reading file") );
			msgBox.setStandardButtons(QMessageBox::Ok);
			msgBox.setDefaultButton(QMessageBox::Ok);
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.exec();
			exit(0);
		}
		else if(error == Database::dbnotexists && !typed)
		{
			int ret;
			do
			{
				QString pass;
				PasswordDialog dialog(&pass,true);
				dialog.setWindowIcon(QIcon(":/qpass.png"));
				ret = dialog.exec();
				if(ret == 1)
				{
					temppassword = pass;
					key = pass;
					typed = true;
					continue;
				}
				else if(ret == 0)
					exit(0);
				else if(ret == 2)
				{
					QMessageBox msgBox;
					msgBox.setWindowTitle("QPass");
					msgBox.setText( tr("Passwords doesn't match") );
					msgBox.setStandardButtons(QMessageBox::Ok);
					msgBox.setDefaultButton(QMessageBox::Ok);
					msgBox.setIcon(QMessageBox::Critical);
					msgBox.exec();
					continue;
				}
				else if(ret == 3)
				{
					newFromImport();
					QMessageBox msgBox;
					msgBox.setWindowIcon(QIcon(":/qpass.png"));
					msgBox.setWindowTitle(tr("QPass"));
					msgBox.setText( tr("Database has been imported succesfully. Password has been set to the same as in imported database.") );
					msgBox.setStandardButtons(QMessageBox::Ok);
					msgBox.setDefaultButton(QMessageBox::Ok);
					msgBox.setIcon(QMessageBox::Information);
					msgBox.exec();
					return 0;
				}
			}while(ret != 1);
		}
		else if(error == Database::dbnotexists && !password.isNull())
		{
			QMessageBox msgBox;
			msgBox.setWindowTitle("QPass");
			msgBox.setText( tr("File does not exist.") );
			msgBox.setStandardButtons(QMessageBox::Ok);
			msgBox.setDefaultButton(QMessageBox::Ok);
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.exec();
			return Database::dbnotexists;
		}
		else if(error == Database::badpass)
		{
			if(!first)
			{
				QMessageBox msgBox;
				msgBox.setWindowTitle("QPass");
				msgBox.setText( tr("Passwords isn't correct") );
				msgBox.setStandardButtons(QMessageBox::Ok);
				msgBox.setDefaultButton(QMessageBox::Ok);
				msgBox.setIcon(QMessageBox::Critical);
				msgBox.exec();
			}
			if(password.isNull())
			{
				first = false;
				QString pass;
				PasswordDialog dialog(&pass,false);
				dialog.setWindowIcon(QIcon(":/qpass.png"));
				int ret = dialog.exec();
				if(ret == 1)
				{
					temppassword = pass;
					key = pass;
					continue;
				}
				else if(ret == 0)
					exit(0);
			}
			else
				return Database::badpass;
		}
		else
			break;
	}while(true);
	
	for(int i = 0; i < data.size(); i++)
	{
		QListWidgetItem *item = new QListWidgetItem();
		item->setData(nameRole, data[i][0]);
		item->setData(urlRole, data[i][1]);
		item->setData(usernameRole, data[i][2]);
		item->setData(passwordRole, data[i][3]);
		item->setData(notesRole, data[i][4]);
		list->addItem(item);
	}
	dataChanged = false;
	return 0;
}

void QPass::createGUI()
{   
	setWindowIcon(QIcon(":/qpass.png"));
	setWindowTitle("QPass");
	QVBoxLayout *vlayout = new QVBoxLayout();
	QWidget *mainWidget = new QWidget();
	setCentralWidget(mainWidget);
	QHBoxLayout *uplayout = new QHBoxLayout();
	QHBoxLayout *downlayout = new QHBoxLayout();
	
	mainWidget->setLayout(vlayout);
	vlayout->addLayout(uplayout);
	vlayout->addLayout(downlayout);
	
	QWidget *buttons = new QWidget();
	QHBoxLayout *buttonsLayout = new QHBoxLayout();
	buttons->setFixedWidth(200);
	buttons->setLayout(buttonsLayout);
	downlayout->setAlignment(Qt::AlignLeft);
	addButton = new QPushButton( tr("Add") );
	buttonsLayout->addWidget(addButton);
	deleteButton = new QPushButton( tr("Delete") );
	buttonsLayout->addWidget(deleteButton);
	downlayout->addWidget(buttons);
	
	uplayout->setAlignment(Qt::AlignLeft);
	list = new QListWidget();
	list->setMaximumWidth(200);
	uplayout->addWidget(list);
	formWidget = new QWidget();
	form = new QFormLayout();
	formWidget->setLayout(form);
	formWidget->hide();
	uplayout->addWidget(formWidget, 1);
	name = new QLineEdit();
	form->addRow( tr("Name:") , name);
	url = new QLineEdit();
	form->addRow( tr("Website URL:") , url);
	copyUrlButton = new QPushButton( tr("Copy URL") );
	form->addRow("", copyUrlButton);
	username = new QLineEdit();
	form->addRow( tr("User name:") , username);
	copyUsernameButton = new QPushButton( tr("Copy user name") );
	form->addRow("", copyUsernameButton);
	password = new QLineEdit();
	password->setEchoMode(QLineEdit::Password);
	passwordVisible = false;
	form->addRow( tr("Password:"), password);
	QHBoxLayout *passwordLay = new QHBoxLayout();
	copyPasswordButton = new QPushButton( tr("Copy password") );
	showPasswordButton = new QPushButton( tr("Show password") );
	passwordLay->addWidget(copyPasswordButton);
	passwordLay->addWidget(showPasswordButton);
	form->addRow("", passwordLay);
	notes = new QTextEdit();
	form->addRow( tr("Notes:") , notes);
	
	connect(list, SIGNAL(itemSelectionChanged()), this, SLOT(changedList()));
	connect(name, SIGNAL(textChanged(QString)), this, SLOT(nameChanged(QString)));
	connect(url, SIGNAL(textChanged(QString)), this, SLOT(urlChanged(QString)));
	connect(username, SIGNAL(textChanged(QString)), this, SLOT(usernameChanged(QString)));
	connect(password, SIGNAL(textChanged(QString)), this, SLOT(passwordChanged(QString)));
	connect(notes, SIGNAL(textChanged()), this, SLOT(notesChanged()));
	connect(copyUrlButton, SIGNAL(clicked()), this, SLOT(copyUrl()));
	connect(copyUsernameButton, SIGNAL(clicked()), this, SLOT(copyUsername()));
	connect(copyPasswordButton, SIGNAL(clicked()), this, SLOT(copyPassword()));
	connect(showPasswordButton, SIGNAL(clicked()), this, SLOT(showPassword()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteCurrentItem()));
	connect(addButton, SIGNAL(clicked()), this, SLOT(addItem()));
	
	QAction *changePasswordAction = new QAction( tr("Change password"), this);
	QAction *exportDatabaseAction = new QAction( tr("Export database"), this);
	QAction *importDatabaseAction = new QAction( tr("Import database"), this);
	hideOnCloseAction = new QAction( tr("Hide on close"), this );
	hideOnCloseAction->setCheckable(true);
	QAction *quitAction = new QAction( tr("Quit") ,this);
	QMenu *dbMenu = new QMenu( tr("Database") );
	dbMenu->addAction(changePasswordAction);
	dbMenu->addAction(exportDatabaseAction);
	dbMenu->addAction(importDatabaseAction);
	dbMenu->addAction(hideOnCloseAction);
	dbMenu->addSeparator();
	dbMenu->addAction(quitAction);
	menuBar()->addMenu(dbMenu);
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(changePasswordAction, SIGNAL(triggered()), this, SLOT(changePassword()));
	connect(exportDatabaseAction, SIGNAL(triggered()), this, SLOT(exportDatabase()));
	connect(importDatabaseAction, SIGNAL(triggered()), this, SLOT(importDatabase()));
	QAction *aboutAction = new QAction( tr("About"), this);
	QMenu *helpMenu = new QMenu( tr("Help") );
	helpMenu->addAction(aboutAction);
	menuBar()->addMenu(helpMenu);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
	trayMenu = new QMenu();
	trayMenu->addAction(quitAction);
	trayIcon = new QSystemTrayIcon(QIcon(":/qpass.png"),this);
	trayIcon->setContextMenu(trayMenu);
	trayIcon->setToolTip( tr("QPass - password manager") );
	trayIcon->show();
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayClicked(QSystemTrayIcon::ActivationReason)));
	readSettings();
}

void QPass::changedList()
{
	if(dataChanged)
		saveData();
	QListWidgetItem *item = list->currentItem();
	formWidget->show();
	name->setText(item->data(nameRole).toString());
	url->setText(item->data(urlRole).toString());
	username->setText(item->data(usernameRole).toString());
	password->setText(item->data(passwordRole).toString());
	notes->setPlainText(item->data(notesRole).toString());
	
	passwordVisible = false;
	password->setEchoMode(QLineEdit::Password);
	showPasswordButton->setText( tr("Show password") );
	dataChanged = false;
}

void QPass::nameChanged(QString text)
{
	list->currentItem()->setData(nameRole, text);
	dataChanged = true;
}

void QPass::urlChanged(QString text)
{
	list->currentItem()->setData(urlRole, text);
	dataChanged = true;
}

void QPass::usernameChanged(QString text)
{
	list->currentItem()->setData(usernameRole, text);
	dataChanged = true;
}

void QPass::passwordChanged(QString text)
{
	list->currentItem()->setData(passwordRole, text);
	dataChanged = true;
}

void QPass::notesChanged()
{
	list->currentItem()->setData(notesRole, notes->toPlainText());
	dataChanged = true;
}

void QPass::copyUrl()
{
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(url->text());
}

void QPass::copyUsername()
{
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(username->text());
}

void QPass::copyPassword()
{
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(password->text());
}

void QPass::showPassword()
{
	if(passwordVisible)
	{
		passwordVisible = false;
		password->setEchoMode(QLineEdit::Password);
		showPasswordButton->setText( tr("Show password") );
	}
	else
	{
		passwordVisible = true;
		password->setEchoMode(QLineEdit::Normal);
		showPasswordButton->setText( tr("Hide password") );
	}
}

void QPass::deleteCurrentItem()
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("QPass");
	msgBox.setText( tr("Are you sure to delete selected entry?") );
	msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
	msgBox.setIcon(QMessageBox::Question);
	if(list->currentRow() >= 0 && list->count() == 1)
	{
		int ret = msgBox.exec();
		if(ret == QMessageBox::Yes)
		{
			list->clear();
			formWidget->hide();
		}
	}
	else if(list->currentRow() >= 0 && list->count() > 1)
	{
		int ret = msgBox.exec();
		if(ret == QMessageBox::Yes)
		{
			delete list->currentItem();
			list->clearSelection();
			formWidget->hide();
		}
	}
	saveData();
}

void QPass::addItem()
{
	QListWidgetItem *item = new QListWidgetItem();
	list->addItem(item);
	list->setCurrentItem(item);
}


int QPass::saveData(QString path)
{
	QList<QStringList> data;
	for(int i = 0; i < list->count(); i++)
	{
		QListWidgetItem *item = list->item(i);
		QStringList entry;
		entry.append(item->data(nameRole).toString());
		entry.append(item->data(urlRole).toString());
		entry.append(item->data(usernameRole).toString());
		entry.append(item->data(passwordRole).toString());
		entry.append(item->data(notesRole).toString());
		data.append(entry);
	}
	Database database;
	if( database.writeData(data, key, path) != 0 )
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("QPass");
		msgBox.setText( tr("Error writing to database") );
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.exec();
		return -1;
	}
	return 0;
	dataChanged = false;
}

void QPass::changePassword()
{
	QDialog passDialog;
	passDialog.setWindowTitle( tr("QPass - password change") );
	QFormLayout *layout = new QFormLayout();
	passDialog.setLayout(layout);
	QLineEdit *oldPass = new QLineEdit();
	oldPass->setEchoMode(QLineEdit::Password);
	QLineEdit *newPass1 = new QLineEdit();
	newPass1->setEchoMode(QLineEdit::Password);
	QLineEdit *newPass2 = new QLineEdit();
	newPass2->setEchoMode(QLineEdit::Password);
	QPushButton *changeButton = new QPushButton( tr("Change") );
	layout->addRow( tr("Old password:"), oldPass);
	layout->addRow( tr("New password:"), newPass1);
	layout->addRow( tr("Repeat new password:"), newPass2);
	layout->addRow( "", changeButton);
	connect(changeButton, SIGNAL(clicked()), &passDialog, SLOT(accept()));
	if(passDialog.exec() == 1)
	{
		QString old = key;
		
		QMessageBox msgBox;
		if(oldPass->text() == old)
		{
			if(newPass1->text() == newPass2->text())
			{
				key = newPass1->text();
				saveData();
				msgBox.setText( tr("Password has been changed succesfully") );
				msgBox.setIcon(QMessageBox::Information);
			}
			else
			{
				msgBox.setText( tr("Passwords doesn't match") );
				msgBox.setIcon(QMessageBox::Warning);
			}
		}
		else
		{
			msgBox.setText( tr("Bad old password") );
			msgBox.setIcon(QMessageBox::Warning);
		}
		if(msgBox.text().size() > 0)
		{
			msgBox.setWindowTitle("QPass");
			msgBox.setStandardButtons(QMessageBox::Ok);
			msgBox.setDefaultButton(QMessageBox::Ok);
			msgBox.exec();
		}
	}
}

void QPass::trayClicked(QSystemTrayIcon::ActivationReason reason)
{
	if(reason == QSystemTrayIcon::Trigger)
	{
		if(isVisible())
			writeSettings();
		setVisible(!isVisible());
		if(isVisible())
			readSettings();
	}
}

void QPass::readSettings()
{
	QSettings settings("QPass", "QPass");
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(650, 500)).toSize();
	hideOnCloseAction->setChecked(settings.value("hideOnClose", false).toBool());
	
	resize(size);
	move(pos);
}

void QPass::writeSettings()
{
	QSettings settings("QPass", "QPass");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
	settings.setValue("hideOnClose", hideOnCloseAction->isChecked());
}

void QPass::closeEvent (QCloseEvent *event)  
{
	if(hideOnCloseAction->isChecked())
	{
		writeSettings();
		setVisible(false);
		event->ignore();
	}
	else
		event->accept();
}

void QPass::about()
{
	AboutWindow *about = new AboutWindow();
	about->exec();
}

void QPass::exportDatabase()
{
	QString path;
	ExportDialog dialog(&path);
	if(dialog.exec() == 1)
	{
		if(saveData(path) == 0)
		{
			QMessageBox msgBox;
			msgBox.setWindowTitle("QPass");
			msgBox.setStandardButtons(QMessageBox::Ok);
			msgBox.setDefaultButton(QMessageBox::Ok);
			msgBox.setIcon(QMessageBox::Information);
			msgBox.setText( tr("Database has been exported succesfully") );
			msgBox.exec();
		}
	}
}

void QPass::importDatabase()
{
	QString path;
	QString password;
	bool method;
	int result;
	while(true)
	{
		ImportDialog dialog(&path, &password, &method);
		if(dialog.exec() == 1)
		{
			if(method == 0)
			{
				if(list->count() > 0)
				{
					QMessageBox msgBox;
					msgBox.setWindowTitle(tr("QPass"));
					msgBox.setText( tr("It will clean your previous entries. Are you sure you want to import database?") );
					msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
					msgBox.setDefaultButton(QMessageBox::No);
					msgBox.setIcon(QMessageBox::Question);
					if(msgBox.exec() == QMessageBox::No)
						return;
				}
				saveData();
				list->clear();
			}
			result = readData(password, path);
			if(result != 0 && method == 0)
				readData();
			if(result == Database::badpass)
				continue;
			else if(result == 0)
			{
				QMessageBox msgBox;
				msgBox.setWindowTitle(tr("QPass"));
				msgBox.setText( tr("Database has been imported succesfully.") );
				msgBox.setStandardButtons(QMessageBox::Ok);
				msgBox.setDefaultButton(QMessageBox::Ok);
				msgBox.setIcon(QMessageBox::Information);
				msgBox.exec();
				dataChanged = true;
				break;
			}
		}
		else
			break;
	}

}

void QPass::newFromImport()
{
	QString path;
	QString password;
	int result;
	while(true)
	{
		ImportDialog dialog(&path, &password);
		dialog.setWindowIcon(QIcon(":/qpass.png"));
		if(dialog.exec() == 1)
		{
			result = readData(password, path);
			if(result == Database::badpass)
				continue;
			else if(result == 0)
			{
				key = password;
				saveData();
				break;
			}
		}
		else
			exit(0);
	}
}
#include "QPass.moc"
