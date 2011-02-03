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

#ifndef PASSWORDCHANGEDIALOG_H
#define PASSWORDCHANGEDIALOG_H

#include "ui_PasswordChangeDialog.h"

#include <QDialog>
#include <QString>

/*!
 * This dialog asks user for current password and new password
 * and verifies that data before accepting dialog.
 */
class PasswordChangeDialog : public QDialog, private Ui::PasswordChangeDialog
{
   Q_OBJECT
   public:
      /*! Constructor of dialog.
       * 
       * It takes current password to check if current password provided by user
       * now match current password.
       */
      PasswordChangeDialog(const QString &currentPassword, QWidget *parent);
      /*! Function returns new password.
       * 
       * Returned password is ready to use.
       * 
       * @return new password.
       */
      QString getNewPassword();
   private:
      QString currentPassword;
   private slots:
      void accept();
};

#endif //PASSWORDCHANGEDIALOG_H
