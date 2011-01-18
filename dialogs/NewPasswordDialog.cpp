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

#include "NewPasswordDialog.h"

NewPasswordDialog::NewPasswordDialog(QWidget *parent) : QDialog(parent)
{
   setupUi(this);
}

QString NewPasswordDialog::value()
{
   return passwordEdit->text();
}

void NewPasswordDialog::accept()
{
   if(passwordEdit->text() != passwordEdit2->text())
   {
      QMessageBox box(this);
      box.setText( tr("Passwords does not match") );
      box.setIcon( QMessageBox::Warning );
      box.exec();
      //done(2);
   }
   else
      done(QDialog::Accepted);
}
