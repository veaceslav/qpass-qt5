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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractItemModel>

#include "DataModel.h"
#include "ui_MainWindow.h"
#include "NewPasswordDialog.h"
#include "PreviousPasswordDialog.h"
#include "NewDatabaseDialog.h"

/*! 
 * This is the main window widget.
 */
class MainWindow : public QMainWindow, private Ui::MainWindow
{
   Q_OBJECT
   public:
      MainWindow(QWidget *parent = 0);
      void showPreviousPasswordDialog();
      void showNewPasswordDialog();
      void showNewDatabaseDialog();
   private:
      DataModel *model;
      QString password;
      QString path;
      PreviousPasswordDialog *previousPasswordDialog;
      NewPasswordDialog *newPasswordDialog;
      NewDatabaseDialog *newDatabaseDialog;
   private slots:
      void initWidgets(int result = 1);
};


#endif