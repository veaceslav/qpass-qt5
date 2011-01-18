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
   private:
      /*! This function shows PreviousPasswordDialog.
       *
       * PreviousPasswordDialog is used to get a password to existing database from user.
       */
      void showPreviousPasswordDialog();
      /*! This function shows NewPasswordDialog.
       *
       * NewPasswordDialog is used to get a password to new database from user.
       */
      void showNewPasswordDialog();
      /*! This function shows NewDatabaseDialog.
       *
       * NewDatabaseDialog is used to determine how user want to create new database,
       * he could create empty database or import existing database.
       */
      void showNewDatabaseDialog();
      
      DataModel *model;
      QItemSelectionModel *selectionModel;
      QString path;
      QString password;
      PreviousPasswordDialog *previousPasswordDialog;
      NewPasswordDialog *newPasswordDialog;
      NewDatabaseDialog *newDatabaseDialog;
   private slots:
      /*!
       * This function inits widgets and shows MainWindow.
       */
      void initWidgets(int result = 1);
      /*!
       * This slot uses insertRows function to add row to database.
       */
      void addItem();
      /*!
       * This slot removes selected item using removeRows function.
       */
      void removeSelectedItem();
      /*!
       * This slot shows data from selected item.
       */
      void showSelectedItem( const QItemSelection & selected, const QItemSelection & deselected );
      void enableSaveButton();
      /*!
       * This slot saves changes from actually selected item
       */
      void saveItem();
};


#endif
