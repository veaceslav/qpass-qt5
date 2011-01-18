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


#ifndef NEWPASSWORDDIALOG_H
#define NEWPASSWORDDIALOG_H

#include <QDialog>

#include "ui_NewPasswordDialog.h"

/*!
 * Provides new password entry dialog.
 */
class NewPasswordDialog : public QDialog, private Ui::NewPasswordDialog
{
   Q_OBJECT
   public:
      NewPasswordDialog(QWidget *parent = 0);
      QString value();
   private slots:
      /*! Own implementation of accect function which compares password edits before accepting.
       *
       * Function compares two password edits and if they aren't equal it shows QMessageBox with warning, otherwise it returns QDialog::Accepted.
       */
      void accept();
};

#endif //NEWPASSWORDDIALOG_H
