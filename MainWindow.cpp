/***************************************************************************
 *   Copyright (c) 2011 Mateusz Piękos <mateuszpiekos@gmail.com>           *
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

#include "MainWindow.h"
#include "DataModel.h"
#include "PredefinedSettings.h"
#include "DatabaseExportDialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
   previousPasswordDialog = NULL;
   newDatabaseDialog = NULL;
   
   //Path to default database $HOME/.qpassdb
   path = PredefinedSettings::databasePath();
   
   if(QFile::exists(path))
      showPreviousPasswordDialog();
   else
      showNewDatabaseDialog();  
}

MainWindow::~MainWindow()
{
   writeSettings();
}

void MainWindow::writeSettings()
{
   QSettings settings;
   settings.setValue("pos", pos());
   settings.setValue("size", size());
}

void MainWindow::readSettings()
{
   QSettings settings;
   QVariant pos = settings.value("pos");
   if(pos.isValid())
      move(pos.toPoint());
   QSize size = settings.value("size", QSize(751, 594)).toSize();
   resize(size);
}

void MainWindow::showPreviousPasswordDialog()
{
   if(previousPasswordDialog == NULL)
   {
      previousPasswordDialog = new PreviousPasswordDialog(this);
      connect(previousPasswordDialog, SIGNAL(accepted()), this, SLOT(init()));
      connect(previousPasswordDialog, SIGNAL(rejected()), qApp, SLOT(quit()));
   }
   previousPasswordDialog->show();
}

void MainWindow::showNewDatabaseDialog()
{
   if(newDatabaseDialog == NULL)
   {
      newDatabaseDialog = new NewDatabaseDialog(this);
      connect(newDatabaseDialog, SIGNAL(accepted()), this, SLOT(init()));
      connect(newDatabaseDialog, SIGNAL(rejected()), qApp, SLOT(quit()));
   }
   newDatabaseDialog->show();
}

void MainWindow::showAboutDialog()
{
   AboutDialog about(this);
   about.exec();
}

void MainWindow::showDatabaseExportDialog()
{
   DatabaseExportDialog exportDialog(this);
   if(exportDialog.exec() == QDialog::Accepted)
   {
      QMessageBox box(this);
      QString exportPath = exportDialog.getPath();
      QString exportPassword = exportDialog.getPassword();
      if(model->exportDatabase( exportPath, exportPassword ))
      {
	 box.setText( tr("Database exported successfully.") );
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

void MainWindow::init()
{
   bool dbExists = QFile::exists(path);
   if(dbExists)
      password = previousPasswordDialog->value();
   else
      password = newDatabaseDialog->value();
   if(dbExists)
   {
      int res = DataModel::checkDatabase(path, password);
      if(res == -1)
      {
	 QMessageBox box(this);
	 box.setWindowTitle( tr("Incorrect password - QPass") );
	 box.setText( tr("Incorrect password.") );
	 box.setIcon(QMessageBox::Warning);
	 box.exec();
	 showPreviousPasswordDialog();
	 return;
      }
      else if(res == -2)
      {
	 QMessageBox box(this);
	 box.setText( tr("Error opening database.") );
	 box.setIcon(QMessageBox::Critical);
	 box.exec();
	 qApp->quit();
      }
   }
   setupUi(this);
   
   readSettings();
   
   model = new DataModel(path, password, dbExists, this);
   listView->setModel(model);
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
   connect(copyurlButton, SIGNAL(clicked()), this, SLOT(copyURL()));
   connect(copyUserNameButton, SIGNAL(clicked()), this, SLOT(copyUserName()));
   connect(copyPasswordButton, SIGNAL(clicked()), this, SLOT(copyPassword()));
   connect(showPasswordButton, SIGNAL(clicked()), this, SLOT(switchEchoMode()));
   
   connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));
   connect(actionExportDatabase, SIGNAL(triggered()), this, SLOT(showDatabaseExportDialog()));
   this->show();
}

void MainWindow::addItem()
{
   model->insertRows(model->rowCount(), 1);
   selectionModel->clearSelection();
   selectionModel->setCurrentIndex( model->index( model->rowCount()-1, 0 ), QItemSelectionModel::SelectCurrent);
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
	 QModelIndex model = list[0];
	 this->model->removeRows(model.row(), 1);
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
      nameEdit->setText( model->data( model->index( row, 0 ) ).toString() );
      urlEdit->setText( model->data( model->index( row, 1 ) ).toString() );
      userNameEdit->setText( model->data( model->index( row, 2 ) ).toString() );
      passwordEdit->setText( model->data( model->index( row, 3 ) ).toString() );
      notesEdit->setText( model->data( model->index( row, 4 ) ).toString() );
      deleteButton->setEnabled(true);
   }
   else
   {
      itemWidget->setEnabled(false);
      nameEdit->setText(QString());
      urlEdit->setText(QString());
      userNameEdit->setText(QString());
      passwordEdit->setText(QString());
      notesEdit->setText(QString());
      deleteButton->setEnabled(false);
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
      if(row >= model->rowCount())
	 return;
   }
   else
      row = selectionModel->selection().indexes()[0].row();
   model->setData( model->index( row, 0), nameEdit->text());
   model->setData( model->index( row, 1), urlEdit->text());
   model->setData( model->index( row, 2), userNameEdit->text());
   model->setData( model->index( row, 3), passwordEdit->text());
   model->setData( model->index( row, 4), notesEdit->toPlainText());
   saveButton->setEnabled(false);
}

void MainWindow::copyURL()
{
   QApplication::clipboard()->setText(urlEdit->text());
}

void MainWindow::copyUserName()
{
   QApplication::clipboard()->setText(userNameEdit->text());
}

void MainWindow::copyPassword()
{
   QApplication::clipboard()->setText(passwordEdit->text());
}

void MainWindow::switchEchoMode()
{
   if(passwordEdit->echoMode() == QLineEdit::Password)
   {
      passwordEdit->setEchoMode( QLineEdit::Normal );
      showPasswordButton->setText( tr("Hide password") );
   }
   else
   {
      passwordEdit->setEchoMode( QLineEdit::Password );
      showPasswordButton->setText( tr("Show password") );
   }
}
