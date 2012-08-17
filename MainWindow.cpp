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

#include <QMessageBox>
#include <QFile>
#include <QClipboard>
#include <QSettings>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QTimer>

#include "MainWindow.h"
#include "DataModel.h"
#include "PredefinedSettings.h"
#include "DatabaseExportDialog.h"
#include "DatabaseImportDialog.h"
#include "PasswordChangeDialog.h"
#include "PasswordGeneratorDialog.h"
#include "PreferencesDialog.h"
#include "PasswordViewer.h"
#include "UpdateCheckerDialog.h"
#include "qpass-config.h"

MainWindow::MainWindow(DataModel *model, QWidget *parent) : QMainWindow(parent)
{
	setWindowIcon(QIcon(":/icons/qpass.png"));

	setupUi(this);
	
	QAction *moveUpAction = new QAction( tr("Move Up"), this);
	moveUpAction->setShortcut( Qt::CTRL + Qt::Key_Up );
	listView->addAction(moveUpAction);

	QAction *moveDownAction = new QAction( tr("Move Down"), this);
	moveDownAction->setShortcut( Qt::CTRL + Qt::Key_Down );
	listView->addAction( moveDownAction );

	connect(moveUpAction, SIGNAL(triggered()), this, SLOT(moveUpEntry()));
	connect(moveDownAction, SIGNAL(triggered()), this, SLOT(moveDownEntry()));
	
	readWindowState();
	
	this->model = model;
	
	proxyModel = new QSortFilterProxyModel(this);
	proxyModel->setSourceModel( model );
	proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
	
	listView->setModel(proxyModel);
	
	selectionModel = listView->selectionModel();
	
	connect(addButton, SIGNAL(clicked()), this, SLOT(addItem()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(removeSelectedItem()));
	connect(selectionModel, SIGNAL(selectionChanged( const QItemSelection &, const QItemSelection &)), this, SLOT(showSelectedItem( const QItemSelection &, const QItemSelection &)));
	connect(nameEdit, SIGNAL(textEdited(const QString &)), this, SLOT(enableSaveButton()));
	connect(urlEdit, SIGNAL(textEdited(const QString &)), this, SLOT(enableSaveButton()));
	connect(userNameEdit, SIGNAL(textEdited(const QString &)), this, SLOT(enableSaveButton()));
	connect(passwordEdit, SIGNAL(textEdited(const QString &)), this, SLOT(enableSaveButton()));
	connect(notesEdit, SIGNAL(textChanged()), this, SLOT(enableSaveButton()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveItem()));
	connect(copyURLButton, SIGNAL(clicked()), this, SLOT(copyURL()));
	connect(goToURLButton, SIGNAL(clicked()), this, SLOT(goToURL()));
	connect(copyUserNameButton, SIGNAL(clicked()), this, SLOT(copyUserName()));
	connect(copyPasswordButton, SIGNAL(clicked()), this, SLOT(copyPassword()));
	connect(showPasswordButton, SIGNAL(clicked()), this, SLOT(switchEchoMode()));
	connect(searchEdit, SIGNAL(textChanged(const QString &)), proxyModel, SLOT(setFilterFixedString(const QString &)));
	connect(deselectButton, SIGNAL(clicked()), selectionModel, SLOT(clearSelection()));
	
	connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
	connect(actionExportDatabase, SIGNAL(triggered()), this, SLOT(exportDatabase()));
	connect(actionImportDatabase, SIGNAL(triggered()), this, SLOT(importDatabase()));
	connect(actionChangePassword, SIGNAL(triggered()), this, SLOT(changePassword()));
	connect(actionQuit, SIGNAL(triggered()), this, SLOT(quit()));
	connect(actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferencesDialog()));
	connect(actionGeneratePassword, SIGNAL(triggered()), this, SLOT(generatePassword()));
	connect(actionCheckForUpdates, SIGNAL(triggered()), this, SLOT(showUpdateChecker()));
	connect(actionFAQ, SIGNAL(triggered()), this, SLOT(openFAQ()));
	connect(actionShowNumberedCharacters, SIGNAL(triggered()), this, SLOT(showPasswordViewer()));
	connect(actionSortAscending, SIGNAL(triggered()), this, SLOT(sortAscending()));
	connect(actionSortDescending, SIGNAL(triggered()), this, SLOT(sortDescending()));

	trayIcon = new TrayIcon(model, this);

	readSettings();

	connect(trayIcon, SIGNAL(clicked()), this, SLOT(showHideWindow()));
	connect(trayIcon, SIGNAL(hideOnCloseTriggered(bool)), this, SLOT(switchHideOnClose(bool)));
	connect(trayIcon, SIGNAL(alwaysOnTopTriggered(bool)), this, SLOT(switchAlwaysOnTop(bool)));
	connect(trayIcon, SIGNAL(quitClicked()), this, SLOT(quit()));

	if(trayIcon->getAlwaysOnTopState())
		setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint );
	
	this->show();

	trayIcon->setHideOnCloseChecked( hideOnClose );
	trayIcon->setVisible(true);

	checker = new UpdateChecker(this);
	connect(checker, SIGNAL(gotLatestVersion(QString)), this, SLOT(informAboutNewVersion(QString)));
	if(lastUpdateCheck.isValid())
	{
		if(lastUpdateCheck != QDate::currentDate())
		{
			checker->checkForUpdates();
			lastUpdateCheck = QDate::currentDate();
		}
	}
}

void MainWindow::writeSettings()
{
#ifdef PORTABLE
	QSettings settings("settings.ini", QSettings::IniFormat);
#else
	QSettings settings;
#endif
	settings.setValue("hideOnClose", hideOnClose);
	settings.setValue("alwaysOnTop", trayIcon->getAlwaysOnTopState());
	settings.setValue("visibleElementsAmount", trayIcon->getVisibleElementsAmount());
	settings.setValue("lastUpdateCheck", lastUpdateCheck);
	settings.setValue("clipboardTimeout", clipboardTimeout);
	settings.setValue("showPasswordByDefault", showPasswordByDefault);
}

void MainWindow::writeWindowState()
{
#ifdef PORTABLE
	QSettings settings("settings.ini", QSettings::IniFormat);
#else
	QSettings settings;
#endif
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}

void MainWindow::readSettings()
{
#ifdef PORTABLE
	QSettings settings("settings.ini", QSettings::IniFormat);
#else
	QSettings settings;
#endif
	clipboardTimeout = settings.value("clipboardTimeout", 0).toInt();
	hideOnClose = settings.value("hideOnClose", false).toBool();
	trayIcon->setAlwaysOnTopState( settings.value("alwaysOnTop", false).toBool() );
	trayIcon->setVisibleElementsAmount( settings.value("visibleElementsAmount", 15).toInt() );
	lastUpdateCheck = settings.value("lastUpdateCheck", QDate()).toDate();
	showPasswordByDefault = settings.value("showPasswordByDefault", false).toBool();
}

void MainWindow::readWindowState()
{
#ifdef PORTABLE
	QSettings settings("settings.ini", QSettings::IniFormat);
#else
	QSettings settings;
#endif
	QVariant pos = settings.value("pos");
	if(pos.isValid())
		move(pos.toPoint());
	QSize size = settings.value("size", QSize(652, 425)).toSize();
	resize(size);
}

void MainWindow::closeEvent (QCloseEvent *event)  
{
	if(hideOnClose)
	{
		writeWindowState();
		setVisible(false);
		event->ignore();
	}
	else
	{
		if(saveButton->isEnabled())
		{
			QMessageBox box(this);
			box.setWindowTitle( tr("Unsaved entry - QPass") );
			box.setText( tr("Selected data entry has been modified\nDo you want to save your changes or discard them?") );
			box.setStandardButtons( QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
			box.setIcon( QMessageBox::Warning );
			int res = box.exec();
			if(res == QMessageBox::Save)
				saveItem();
			else if(res == QMessageBox::Cancel)
			{
				event->ignore();
				return;
			}
		}
		if(isVisible())
			writeWindowState();
		
		writeSettings();
		event->accept(); 
	}
}

void MainWindow::showAboutDialog()
{
	AboutDialog about(this);
	about.exec();
}

void MainWindow::showPreferencesDialog()
{
	PreferencesDialog preferences(this);

	preferences.setVisibleElementsAmount( trayIcon->getVisibleElementsAmount() );
	preferences.setClipboardTimeout( clipboardTimeout );
	preferences.setShowPassword( showPasswordByDefault );
	preferences.setNumberOfIterations( model->getNumberOfIterations() );
	if( preferences.exec() == QDialog::Accepted )
	{
		trayIcon->setVisibleElementsAmount( preferences.getVisibleElementsAmount() );
		clipboardTimeout = preferences.getClipboardTimeout();
		showPasswordByDefault = preferences.getShowPassword();
		if(preferences.getNumberOfIterations() != model->getNumberOfIterations())
			model->setNumberOfIterations(preferences.getNumberOfIterations());
	}
}

void MainWindow::showUpdateChecker()
{
	UpdateCheckerDialog updateChecker(this);
	updateChecker.setAutomaticCheckingChecked( lastUpdateCheck.isValid() );
	updateChecker.exec();
	if(updateChecker.getAutomaticCheckingChecked())
		lastUpdateCheck = QDate::currentDate();
	else
		lastUpdateCheck = QDate();
}

void MainWindow::showPasswordViewer()
{
	PasswordViewer viewer(this, passwordEdit->text());
	viewer.exec();
}

void MainWindow::exportDatabase()
{
	selectionModel->clearSelection();
	DatabaseExportDialog exportDialog(this);
	if(exportDialog.exec() == QDialog::Accepted)
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("QPass") );
		errorCode err = model->exportDatabase( exportDialog.getPath(), exportDialog.getPassword(), exportDialog.getFormat(), exportDialog.getColumnOrganization());
		if(err == SUCCESS)
		{
			box.setText( tr("Database has been exported successfully.") );
			box.setIcon(QMessageBox::Information);
		}
		else
		{
			box.setText( tr("Error exporting database.") );
			box.setIcon(QMessageBox::Critical);
		}
		box.exec();
	}
}

void MainWindow::importDatabase()
{
	selectionModel->clearSelection();
	DatabaseImportDialog importDialog(this);
	if(importDialog.exec() == QDialog::Accepted)
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("QPass") );
		bool replace = false;
		if(importDialog.getMode() == DatabaseImportDialog::Replace)
		{
			replace = true;
			QMessageBox askBox(this);
			askBox.setWindowTitle( tr("QPass") );
			askBox.setText( tr("All your existing enries will be replaced. Are you sure you want to continue?") );
			askBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
			askBox.setIcon( QMessageBox::Question );
			if( askBox.exec() == QMessageBox::No )
				return;
		}

		int format = importDialog.getFormat();
		int res = model->importDatabase(importDialog.getPath(), importDialog.getPassword(), replace, format, importDialog.getColumnOrganization());
		if( res == SUCCESS )
		{
			box.setText( tr("Database has been imported successfully.") );
			box.setIcon(QMessageBox::Information);
		}
		else
		{
			box.setText( tr("Error importing database. Probably file is corrupted or has incorrect format") );
			box.setIcon(QMessageBox::Critical);
		}
		box.exec();
	}
}

void MainWindow::changePassword()
{
	PasswordChangeDialog passwordDialog(model->getPassword(), this);
	if(passwordDialog.exec() == QDialog::Accepted)
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("QPass") );
		if( model->changePassword( passwordDialog.getNewPassword() ) == SUCCESS )
		{
			box.setText( tr("Password has been changed successfully.") );
			box.setIcon( QMessageBox::Information ); 
		}
		else
		{
			box.setText( tr("Error changing password.") );
			box.setIcon( QMessageBox::Critical ); 
		}
		box.exec();
	}
}

void MainWindow::showHideWindow()
{
	if(isVisible())
	{
		writeWindowState();
		hide();
	}
	else
	{
		show();
		activateWindow();
	}
}

void MainWindow::generatePassword()
{
	PasswordGeneratorDialog dialog(this);

	if( selectionModel->selection().indexes().count() == 0)
		dialog.setSetAsPasswordEnabled(false);
	else
		dialog.setSetAsPasswordEnabled(true);

	if(dialog.exec() == QDialog::Accepted)
	{
		if(dialog.getResult() != QString::Null())
		{
			if(!passwordEdit->text().isEmpty())
			{
				QMessageBox box(this);
				box.setWindowTitle( tr("QPass") );
				box.setText( tr("Current password is not empty. Are you sure you want to overwrite it?") );
				box.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
				box.setIcon( QMessageBox::Question );
				if(box.exec() == QMessageBox::No)
					return;
			}
			passwordEdit->setText( dialog.getResult() );
			enableSaveButton();
		}
	}
}

void MainWindow::quit()
{
	if(saveButton->isEnabled())
	{
		show();
		QMessageBox box(this);
		box.setWindowTitle( tr("Unsaved entry - QPass") );
		box.setText( tr("Selected data entry has been modified\nDo you want to save your changes or discard them?") );
		box.setStandardButtons( QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		box.setIcon( QMessageBox::Warning );
		int res = box.exec();
		if(res == QMessageBox::Save)
			saveItem();
		else if(res == QMessageBox::Cancel)
			return;
	}
	if(isVisible())
		writeWindowState();
	writeSettings();
	qApp->quit();
}

void MainWindow::sortAscending()
{
	sortEntries(Qt::AscendingOrder);
}

void MainWindow::sortDescending()
{
	sortEntries(Qt::DescendingOrder);
}

void MainWindow::sortEntries(Qt::SortOrder order)
{
	selectionModel->clearSelection();
	model->sort(0, order);
	errorCode err = model->saveDatabase();
	if(err == FILE_ERROR)
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("QPass") );
		box.setText("Error writing to database!");
		box.setIcon( QMessageBox::Critical );
		box.exec();
	}
	else if(err == GCRYPT_ERROR)
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("QPass") );
		box.setText("libgcrypt library error");
		box.setIcon( QMessageBox::Critical );
		box.exec();
	}
}

void MainWindow::addItem()
{
	searchEdit->clear();
	selectionModel->clearSelection();
	// check if last entry is empty. If it is, we only 
	// select it instead of adding new
	bool isEmpty = true;
	int row = proxyModel->rowCount()-1;
	for(int i = 0; i < 5; i++)
	{
		if(!proxyModel->data( proxyModel->index( row, i ) ).toString().isEmpty())
			isEmpty = false;
	}

	if(!isEmpty || row == -1)
	{
		if( !proxyModel->insertRows(proxyModel->rowCount(), 1) )
		{
			QMessageBox box(this);
			box.setWindowTitle( tr("QPass") );
			box.setText( tr("Error adding entry.") );
			box.setIcon( QMessageBox::Critical );
			box.exec();
		}
	}

	selectionModel->setCurrentIndex( proxyModel->index( proxyModel->rowCount()-1, 0 ), QItemSelectionModel::SelectCurrent);

    // Give focus to the name box so user can start typing right away
    nameEdit->setFocus();
}

void MainWindow::removeSelectedItem()
{
	QModelIndexList list = selectionModel->selection().indexes();
	if(list.count() == 1)
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("Item remove - QPass") );
		box.setIcon( QMessageBox::Warning );
		box.setStandardButtons( QMessageBox::Yes | QMessageBox::No);
		box.setText( tr("Are you sure to delete selected entry?") );
		if(box.exec() == QMessageBox::Yes)
		{
			saveButton->setEnabled(false);
			QModelIndex modelIndex = list[0];
			if( !this->proxyModel->removeRows(modelIndex.row(), 1) )
			{
				QMessageBox box1(this);
				box1.setWindowTitle( tr("QPass") );
				box1.setText( tr("Error removing entry.") );
				box1.setIcon( QMessageBox::Critical );
				box1.exec();
			}

			errorCode err = model->saveDatabase();
			if(err == FILE_ERROR)
			{
				QMessageBox box(this);
				box.setWindowTitle( tr("QPass") );
				box.setText("Error writing to database!");
				box.setIcon( QMessageBox::Critical );
				box.exec();
			}
			else if(err == GCRYPT_ERROR)
			{
				QMessageBox box(this);
				box.setWindowTitle( tr("QPass") );
				box.setText("libgcrypt library error");
				box.setIcon( QMessageBox::Critical );
				box.exec();
			}
		}
	}
}

void MainWindow::showSelectedItem( const QItemSelection & selected, const QItemSelection & deselected )
{
	if(saveButton->isEnabled())
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("Unsaved entry - QPass") );
		box.setText( tr("Selected data entry has been modified\nDo you want to save your changes or discard them?") );
		box.setStandardButtons( QMessageBox::Save | QMessageBox::Discard);
		box.setIcon( QMessageBox::Warning );
		int res = box.exec();
		if(res == QMessageBox::Save)
			saveItem(deselected.indexes()[0]);
	}
	QModelIndexList list = selected.indexes();
	if(list.count() == 1)
	{
		int row = list[0].row();
		itemWidget->setEnabled(true);
		nameEdit->setText( proxyModel->data( proxyModel->index( row, 0 ) ).toString() );
		urlEdit->setText( proxyModel->data( proxyModel->index( row, 1 ) ).toString() );
		userNameEdit->setText( proxyModel->data( proxyModel->index( row, 2 ) ).toString() );
		passwordEdit->setText( proxyModel->data( proxyModel->index( row, 3 ) ).toString() );
		notesEdit->setPlainText( proxyModel->data( proxyModel->index( row, 4 ) ).toString() );
		deleteButton->setEnabled(true);
		actionShowNumberedCharacters->setEnabled(true);
	}
	else
	{
		itemWidget->setEnabled(false);
		nameEdit->setText(QString());
		urlEdit->setText(QString());
		userNameEdit->setText(QString());
		passwordEdit->setText(QString());
		notesEdit->setPlainText(QString());
		deleteButton->setEnabled(false);
		actionShowNumberedCharacters->setEnabled(false);
	}
	
	//Hides passwords
	if(!showPasswordByDefault)
	{
		passwordEdit->setEchoMode( QLineEdit::Password );
		showPasswordButton->setChecked(false);
	}
	else
	{
		passwordEdit->setEchoMode( QLineEdit::Normal );
		showPasswordButton->setChecked(true);
	}
	
	saveButton->setEnabled(false);
}

void MainWindow::enableSaveButton()
{
	saveButton->setEnabled(true);
}

void MainWindow::saveItem(const QModelIndex &item)
{
	int row;
	if(item.isValid())
	{
		row = item.row();
		if(row >= proxyModel->rowCount())
			return;
	}
	else
		row = selectionModel->selection().indexes()[0].row();
	if( !proxyModel->setData( proxyModel->index( row, 0), nameEdit->text()) )
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("QPass") );
		box.setText("Error writing entry!");
		box.setIcon( QMessageBox::Critical );
		box.exec();
	}
	proxyModel->setData( proxyModel->index( row, 1), urlEdit->text());
	proxyModel->setData( proxyModel->index( row, 2), userNameEdit->text());
	proxyModel->setData( proxyModel->index( row, 3), passwordEdit->text());
	proxyModel->setData( proxyModel->index( row, 4), notesEdit->toPlainText());

	errorCode err = model->saveDatabase();
	if(err == FILE_ERROR)
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("QPass") );
		box.setText("Error writing to database!");
		box.setIcon( QMessageBox::Critical );
		box.exec();
	}
	else if(err == GCRYPT_ERROR)
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("QPass") );
		box.setText("libgcrypt library error");
		box.setIcon( QMessageBox::Critical );
		box.exec();
	}
	saveButton->setEnabled(false);
}

void MainWindow::copyURL()
{
	QApplication::clipboard()->setText(urlEdit->text());
}

void MainWindow::goToURL()
{
	QString url = urlEdit->text();
	if( !(url.indexOf("http://") == 0 || url.indexOf("https://") == 0) )
		url.insert(0, "http://");
	if( !QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode)) )
	{
		QMessageBox box(this);
		box.setWindowTitle("QPass");
		box.setText( tr("Failed to open URL") );
		box.setIcon( QMessageBox::Warning );
		box.exec();
	}
}

void MainWindow::copyUserName()
{
	QApplication::clipboard()->setText(userNameEdit->text());
	if(clipboardTimeout > 0)
		QTimer::singleShot(clipboardTimeout*1000, this, SLOT(clearClipboard()));
}

void MainWindow::copyPassword()
{
	QApplication::clipboard()->setText(passwordEdit->text());
	if(clipboardTimeout > 0)
		QTimer::singleShot(clipboardTimeout*1000, this, SLOT(clearClipboard()));
}

void MainWindow::switchEchoMode()
{
	if(passwordEdit->echoMode() == QLineEdit::Password)
	{
		passwordEdit->setEchoMode( QLineEdit::Normal );
		showPasswordButton->setChecked(true);
	}
	else
	{
		passwordEdit->setEchoMode( QLineEdit::Password );
		showPasswordButton->setChecked(false);
	}
}

void MainWindow::switchHideOnClose(bool checked)
{
	hideOnClose = checked;
	writeSettings();
}

void MainWindow::switchAlwaysOnTop(bool alwaysOnTop)
{
	if(alwaysOnTop)
	{
		if( isVisible() )
			writeWindowState();
		setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint );
		readWindowState();
		show();
	}
	else
	{
		bool visible = isVisible();
		if( visible )
			writeWindowState();
		setWindowFlags( windowFlags() & !Qt::WindowStaysOnTopHint);
		readWindowState();
		setVisible(visible);
	}
	writeSettings();
}

void MainWindow::moveUpEntry()
{
	QModelIndexList list = selectionModel->selection().indexes();

	if( list.count() == 0 )
		return;

	if( !searchEdit->text().isEmpty() )
	{
		QMessageBox box(this);
		box.setText( tr("You can't do this is searching mode") );
		box.setIcon( QMessageBox::Warning );
		box.exec();
		return;
	}

	if(saveButton->isEnabled())
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("Unsaved entry - QPass") );
		box.setText( tr("Selected data entry has been modified\nDo you want to save your changes or discard them?") );
		box.setStandardButtons( QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		box.setIcon( QMessageBox::Warning );
		int res = box.exec();
		if(res == QMessageBox::Save)
			saveItem();
		else if(res == QMessageBox::Cancel)
		{
			return;
		}
		else if(res == QMessageBox::Discard)
		{
			saveButton->setEnabled( false );
		}
	}

	if( list.at(0).row() == 0 )
		return;

	model->swapEntries( list.at(0).row(), list.at(0).row() - 1 );
	
	selectionModel->clearSelection();
	selectionModel->setCurrentIndex( proxyModel->index( list.at(0).row() - 1, 0 ), QItemSelectionModel::SelectCurrent);
}

void MainWindow::moveDownEntry()
{
	QModelIndexList list = selectionModel->selection().indexes();

	if( list.count() == 0 )
		return;

	if( !searchEdit->text().isEmpty() )
	{
		QMessageBox box(this);
		box.setText( tr("You can't do this is searching mode") );
		box.setIcon( QMessageBox::Warning );
		box.exec();
		return;
	}

	if(saveButton->isEnabled())
	{
		QMessageBox box(this);
		box.setWindowTitle( tr("Unsaved entry - QPass") );
		box.setText( tr("Selected data entry has been modified\nDo you want to save your changes or discard them?") );
		box.setStandardButtons( QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		box.setIcon( QMessageBox::Warning );
		int res = box.exec();
		if(res == QMessageBox::Save)
			saveItem();
		else if(res == QMessageBox::Cancel)
		{
			return;
		}
		else if(res == QMessageBox::Discard)
		{
			saveButton->setEnabled( false );
		}
	}

	if( list.at(0).row()+1 == model->rowCount() )
		return;

	model->swapEntries( list.at(0).row(), list.at(0).row() + 1 );
	
	selectionModel->clearSelection();
	selectionModel->setCurrentIndex( proxyModel->index( list.at(0).row() + 1, 0 ), QItemSelectionModel::SelectCurrent);
}

void MainWindow::informAboutNewVersion(QString version)
{
	if(version != VERSION && version != QString::Null())
	{
		QMessageBox box(this);
		box.setWindowTitle("QPass");
		box.setIcon(QMessageBox::Information);
		box.setText( tr("There is new version of QPass available to download.<br />Go to <a href=\"http://qpass.sourceforge.net\">http://qpass.sourceforge.net</a> to get more information") );
		box.setTextFormat(Qt::RichText);
		box.exec();
	}
	checker->deleteLater();
}

void MainWindow::openFAQ()
{
	QDesktopServices::openUrl(QUrl("http://qpass.sourceforge.net/wiki/index.php/FAQ"));
}

void MainWindow::clearClipboard() 
{
	QApplication::clipboard()->clear();
}

bool MainWindow::isUnsaved()
{
	return saveButton->isEnabled();
}
