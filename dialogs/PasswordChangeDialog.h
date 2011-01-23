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

#ifndef PASSWORDCHANGEDIALOG_H
#define PASSWORDCHANGEDIALOG_H

#include "ui_PasswordChangeDialog.h"

#include <QDialog>
#include <QString>

class PasswordChangeDialog : public QDialog, private Ui::PasswordChangeDialog
{
   Q_OBJECT
   public:
      PasswordChangeDialog(const QString &currentPassword, QWidget *parent);
      QString getNewPassword();
   private:
      QString currentPassword;
   private slots:
      void accept();
};

#endif //PASSWORDCHANGEDIALOG_H
