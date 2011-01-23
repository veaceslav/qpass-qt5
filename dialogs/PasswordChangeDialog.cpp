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

#include "PasswordChangeDialog.h"

#include <QMessageBox>

PasswordChangeDialog::PasswordChangeDialog(const QString &currentPassword, QWidget *parent) : QDialog(parent)
{
   setupUi(this);
   this->currentPassword = currentPassword;
}

QString PasswordChangeDialog::getNewPassword()
{
   return newPasswordEdit1->text();
}

void PasswordChangeDialog::accept()
{
   if(newPasswordEdit1->text() != newPasswordEdit2->text())
   {
      QMessageBox box(this);
      box.setText( tr("Passwords does not match") );
      box.setIcon( QMessageBox::Warning );
      box.exec();
      return;
   }
   if(currentPasswordEdit->text() != currentPassword)
   {
      QMessageBox box(this);
      box.setText( tr("Incorrect current password.") );
      box.setIcon(QMessageBox::Warning);
      box.exec();
      return;
   }
   done(QDialog::Accepted);
}
