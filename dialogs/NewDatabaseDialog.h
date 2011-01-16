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

#ifndef NEWDATABASEDIALOG_H
#define NEWDATABASEDIALOG_H

#include <QDialog>

#include "ui_NewDatabaseDialog.h"

/*!
 * Provides dialog to prompt how to create new database.
 */
class NewDatabaseDialog : public QDialog, private Ui::NewDatabaseDialog
{
   Q_OBJECT
   public:
      NewDatabaseDialog(QWidget *parent = 0);
   private slots:
      /*!
       * This function sets result code to 2(Create new database was clicked).
       */
      void acceptNew();
      /*!
       * This function sets result code to 3(Import existing database was clicked).
       */
      void acceptImport();
};


#endif //NEWDATABASEDIALOG_H