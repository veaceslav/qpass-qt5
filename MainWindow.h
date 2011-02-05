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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractItemModel>

#include "DataModel.h"
#include "ui_MainWindow.h"
#include "PreviousPasswordDialog.h"
#include "NewDatabaseDialog.h"
#include "AboutDialog.h"
#include "TrayIcon.h"

/*! 
 * This is the main window widget.
 */
class MainWindow : public QMainWindow, private Ui::MainWindow
{
   Q_OBJECT
   public:
      MainWindow(QWidget *parent = 0);
   private:
      DataModel *model;
      QItemSelectionModel *selectionModel;
      QString path;
      PreviousPasswordDialog *previousPasswordDialog;
      NewDatabaseDialog *newDatabaseDialog;
      TrayIcon *trayIcon;
      bool hideOnClose;
      void writeSettings();
      void writeWindowState();
      void readSettings();
      void readWindowState();
   protected:
      void closeEvent(QCloseEvent * event);
      void hideEvent ( QHideEvent * event );
   private slots:
      /*! This function shows PreviousPasswordDialog.
       *
       * PreviousPasswordDialog is used to get a password to existing database from user.
       */
      void showPreviousPasswordDialog();
      /*! This function shows NewDatabaseDialog.
       *
       * NewDatabaseDialog is used to determine how user want to create new database,
       * he could create empty database or import existing database.
       */
      void showNewDatabaseDialog();
      void showAboutDialog();
      void exportDatabase();
      void importDatabase();
      void changePassword();
      void showHideWindow();
      void generatePassword();
      void quit();
      /*!
       * This function inits widgets and shows MainWindow.
       * 
       * It is executed after previousPasswordDialog or newDatabaseDialog
       * emit accept().
       */
      void init();
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
      /*! Enables save button
       * 
       * State of save button is used to determine if entry was edited or not.
       */
      void enableSaveButton();
      /*!
       * This slot saves changes on actually selected item or provided item.
       *
       * @param item Item to save.
       */
      void saveItem(const QModelIndex &item = QModelIndex());
      void copyURL();
      void copyUserName();
      void copyPassword();
      /*!
       * This slot switches echo mode between normal and password for passwordEdit.
       */
      void switchEchoMode();
      void switchHideOnClose(bool checked);
};


#endif
