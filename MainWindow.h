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
#include <QSortFilterProxyModel>
#include <QDate>

#include "ui_MainWindow.h"
#include "DataModel.h"
#include "AboutDialog.h"
#include "TrayIcon.h"
#include "UpdateChecker.h"

/*! 
* This is the main window widget.
*/
class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT;
	public:
		/*! 
		 * Data passed to this function should be checked before and
		 * should be correct.
		 */
		MainWindow(QString path, QString password, bool dbExists, QWidget *parent = 0);
	private:
		/*! All data should be accesed through proxyModel, only functions such as exportDatabase,
		 * importDatabase, changePassword etc. should be accessed directly through model.
		 */
		DataModel *model;
		QSortFilterProxyModel *proxyModel;
		QItemSelectionModel *selectionModel;
		TrayIcon *trayIcon;
		UpdateChecker *checker;
		bool hideOnClose;
		bool showPasswordByDefault;
		/*!
		 * Time after which clipbard should be cleared
		 * Disable on 0
		 */
		int clipboardTimeout; 
		void writeSettings();
		void writeWindowState();
		void readSettings();
		void readWindowState();
		/*! 
		 * This variable contains date of last check for update.
		 * If lastUpdateCheck is valid then periodic update checking is enabled.
		 */
		QDate lastUpdateCheck;
	protected:
		void closeEvent(QCloseEvent * event);
	private slots:
		void showAboutDialog();
		void showPreferencesDialog();
		void showUpdateChecker();
		void showPasswordViewer();
		void exportDatabase();
		void importDatabase();
		void changePassword();
		void showHideWindow();
		void generatePassword();
		void quit();
		void sortAscending();
		void sortDescending();
		void sortEntries(Qt::SortOrder order);
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
		void goToURL();
		void copyUserName();
		void copyPassword();
		/*!
		* This slot switches echo mode between normal and password for passwordEdit.
		*/
		void switchEchoMode();
		void switchHideOnClose(bool checked);
		void switchAlwaysOnTop(bool alwaysOnTop);
		void moveUpEntry();
		void moveDownEntry();
		void informAboutNewVersion(QString version);
	    void openFAQ();
		void clearClipboard();
};


#endif
