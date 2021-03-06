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

#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QAbstractItemModel>
#include <QAction>
#include <QMenu>

/*! 
* This class provides a icon in system tray.
*/
class TrayIcon : public QSystemTrayIcon
{
	Q_OBJECT
	public:
		TrayIcon(QAbstractItemModel *model, QObject *parent);
		~TrayIcon();
		/*! Changes state of "Hide on close" checkbox.
		* 
		* @param checked state of "Hide on close"
		*/
		void setHideOnCloseChecked(bool checked);
		void setAlwaysOnTopState(bool checked);
		bool getAlwaysOnTopState();
		int getVisibleElementsAmount();
		void setVisibleElementsAmount(int amount);
	private:
		QMenu *menu;
		QAction *hideOnCloseAction;
		QAction *alwaysOnTopAction;
		QAbstractItemModel *model;
		int visibleElementsAmount;
		void changeVisibleElementsAmount(int amount);
	private slots:
		void handleActivated(QSystemTrayIcon::ActivationReason reason);
		void changeData(const QModelIndex &topLeft, const QModelIndex &bottomRight);
		void copyDataToClipboard(QAction *action);
		void insertRows(const QModelIndex &parent, int start, int end);
		void removeRows(const QModelIndex &parent, int start, int end);
	signals:
		/*! 
		* Signal emmited when user clicks tray icon.
		*/
		void clicked();
		/*!Signal emmited when user change state of "Hide on close" checkbox.
		* 
		* @param checked state of checkbox.
		*/
		void hideOnCloseTriggered(bool checked);
		void alwaysOnTopTriggered(bool checked);
		/*! 
		* Signal emmited when user clicks quit action.
		*/
		void quitClicked();
};
#endif //TRAYICON_H
