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

#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QAbstractItemModel>
#include <QAction>
#include <QMenu>

class TrayIcon : public QSystemTrayIcon
{
   Q_OBJECT
   public:
      TrayIcon(QAbstractItemModel *model, QObject *parent);
      ~TrayIcon();
      void setHideOnCloseChecked(bool checked);
   private:
      QAbstractItemModel *model;
      QMenu *menu;
      QAction *hideOnCloseAction;
      QList<QAction *> actionList;
   private slots:
      void handleActivated(QSystemTrayIcon::ActivationReason reason);
   signals:
      void clicked();
      void hideOnCloseTriggered(bool checked);
      void quitClicked();
};
#endif //TRAYICON_H
