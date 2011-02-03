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
   else if(replaceButton->isChecked())
      return DatabaseImportDialog::Replace;
}

void DatabaseImportDialog::browse()
{
   QFileDialog dialog(this);
   dialog.setFilter( tr("QPass database files (*.qpa);;All Files (*.*)") );
   dialog.setFileMode( QFileDialog::ExistingFile );
   if(dialog.exec() == QDialog::Accepted)
      pathEdit->setText( dialog.selectedFiles().at(0));
}

void DatabaseImportDialog::accept()
{
   if(pathEdit->text().isEmpty())
   {
      QMessageBox box(this);
      box.setText( tr("File path can not be empty") );
      box.setIcon( QMessageBox::Warning );
      box.exec();
      return;
   }
   if(DataModel::checkDatabase( getPath(), getPassword() ))
   {
      QMessageBox box(this);
      box.setText( tr("Incorrect password.") );
      box.setIcon(QMessageBox::Warning);
      box.exec();
      return;
   }
   done(QDialog::Accepted);
}
