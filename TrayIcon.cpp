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

#include "TrayIcon.h"

#include <QApplication>

TrayIcon::TrayIcon(QAbstractItemModel *model, QObject *parent) : QSystemTrayIcon(parent)
{
   this->model = model;
   setIcon( QIcon(":/icons/qpass.png") );
   setToolTip( tr("QPass password manager") );
   menu = new QMenu();
   hideOnCloseAction = new QAction(this);
   hideOnCloseAction->setText( tr("Hide on close") );
   hideOnCloseAction->setCheckable(true);
   menu->addAction(hideOnCloseAction);
   menu->addSeparator();
   QAction *quitAction = new QAction(this);
   quitAction->setText( tr("Quit") );
   menu->addAction(quitAction);
   setContextMenu(menu);
   connect(hideOnCloseAction, SIGNAL(triggered(bool)), this, SIGNAL(hideOnCloseTriggered(bool)));
   connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
   connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(handleActivated(QSystemTrayIcon::ActivationReason)));
}

TrayIcon::~TrayIcon()
{
   delete menu;
}

void TrayIcon::setHideOnCloseChecked(bool checked)
{
   hideOnCloseAction->setChecked(checked);
}

void TrayIcon::handleActivated(QSystemTrayIcon::ActivationReason reason)
{
   if(reason == QSystemTrayIcon::Trigger)
      emit clicked();
}
