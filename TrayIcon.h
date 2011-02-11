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
#include <QAction>
#include <QMenu>

/*! 
* This class provides a icon in system tray.
*/
class TrayIcon : public QSystemTrayIcon
{
	Q_OBJECT
	public:
		TrayIcon(QObject *parent);
		~TrayIcon();
		/*! Changed state of "Hide on close" checkbox.
		* 
		* @param checked State of "Hide on close"
		*/
		void setHideOnCloseChecked(bool checked);
	private:
		QMenu *menu;
		QAction *hideOnCloseAction;
	private slots:
		void handleActivated(QSystemTrayIcon::ActivationReason reason);
	signals:
		/*! 
		* Signal emmited when user clicks tray icon.
		*/
		void clicked();
		/*!Signal emmited when users change state of "Hide on close" checkbox.
		* 
		* @param checked State of checkbox.
		*/
		void hideOnCloseTriggered(bool checked);
		/*! 
		* Signal emmited when user clicks quit action.
		*/
		void quitClicked();
};
#endif //TRAYICON_H
