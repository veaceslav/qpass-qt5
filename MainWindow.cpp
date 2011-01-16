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

#include "MainWindow.h"
#include "DataModel.h"
#include "DataAccess.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
   setupUi(this);
   
   previousPasswordDialog = NULL;
   newPasswordDialog = NULL;
   newDatabaseDialog = NULL;
   path = getenv("HOME");
   path += "/baza";
   
   if(QFile::exists(path))
      showPreviousPasswordDialog();
   else
      showNewDatabaseDialog();
      
}

void MainWindow::showPreviousPasswordDialog()
{
   if(previousPasswordDialog == NULL)
   {
      previousPasswordDialog = new PreviousPasswordDialog(this);
      connect(previousPasswordDialog, SIGNAL(accepted()), this, SLOT(initWidgets()));
      connect(previousPasswordDialog, SIGNAL(rejected()), qApp, SLOT(quit()));
   }
   previousPasswordDialog->show();
}

void MainWindow::showNewPasswordDialog()
{
   if(newPasswordDialog == NULL)
   {
      newPasswordDialog = new NewPasswordDialog(this);
      connect(newPasswordDialog, SIGNAL(finished(int)), this, SLOT(initWidgets(int)));
      connect(newPasswordDialog, SIGNAL(rejected()), qApp, SLOT(quit()));
   }
   newPasswordDialog->show();
}

void MainWindow::showNewDatabaseDialog()
{
   if(newDatabaseDialog == NULL)
   {
      newDatabaseDialog = new NewDatabaseDialog(this);
      connect(newDatabaseDialog, SIGNAL(finished(int)), this, SLOT(initWidgets(int)));
      connect(newDatabaseDialog, SIGNAL(rejected()), qApp, SLOT(quit()));
   }
   newDatabaseDialog->show();
}

void MainWindow::initWidgets(int result)
{
   if(QFile::exists(path))
      password = previousPasswordDialog->value();
   else
   {
      if(result == 2)
      {
	 showNewPasswordDialog();
	 return;
      }  
      password = newPasswordDialog->value();
   }
   DataAccess data(path);
   int res = data.open(password);
   data.close();
   if(res == -1)
   {
      QMessageBox box;
      box.setText( tr("Incorrect password.") );
      box.setIcon(QMessageBox::Warning);
      box.exec();
      showPreviousPasswordDialog();
      return;
   }
   else if(res == -2)
   {
      QMessageBox box;
      box.setText( tr("Error opening database.") );
      box.setIcon(QMessageBox::Critical);
      box.exec();
      qApp->quit();
   }
   model = new DataModel(path, password, this);
   listView->setModel(model);
   tableView->setModel(model);
   this->show();
}