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

#include <QApplication>
#include <QClipboard>

#include "TrayIcon.h"

TrayIcon::TrayIcon(QAbstractItemModel *model, QObject *parent) : QSystemTrayIcon(parent)
{
	setIcon( QIcon(":/icons/qpass.png") );
	setToolTip( tr("QPass password manager") );

	visibleElementsAmount = 0;
		
	menu = new QMenu();

	this->model = model;

	menu->addSeparator();

	hideOnCloseAction = new QAction(this);
	hideOnCloseAction->setText( tr("Hide on close") );
	hideOnCloseAction->setCheckable(true);
	menu->addAction(hideOnCloseAction);

	alwaysOnTopAction = new QAction(this);
	alwaysOnTopAction->setText( tr("Always on top") );
	alwaysOnTopAction->setCheckable(true);
	menu->addAction(alwaysOnTopAction);

	menu->addSeparator();

	QAction *quitAction = new QAction(this);
	quitAction->setText( tr("Quit") );
	menu->addAction(quitAction);

	setContextMenu(menu);

	connect(hideOnCloseAction, SIGNAL(triggered(bool)), this, SIGNAL(hideOnCloseTriggered(bool)));
	connect(alwaysOnTopAction, SIGNAL(toggled(bool)), this, SIGNAL(alwaysOnTopTriggered(bool)));
	connect(quitAction, SIGNAL(triggered()), this, SIGNAL(quitClicked()));
	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(handleActivated(QSystemTrayIcon::ActivationReason)));
	connect(model, SIGNAL(dataChanged(const QModelIndex, const QModelIndex)), this,  SLOT(changeData(const QModelIndex, const QModelIndex)));
	connect(model, SIGNAL(rowsInserted(const QModelIndex , int, int)), this, SLOT(insertRows(const QModelIndex , int, int)));
	connect(model, SIGNAL(rowsRemoved(const QModelIndex , int, int)), this, SLOT(removeRows(const QModelIndex , int, int)));
	connect(menu, SIGNAL(triggered(QAction *)), this, SLOT(copyDataToClipboard(QAction *)));
}

TrayIcon::~TrayIcon()
{
	delete menu;
}

void TrayIcon::setHideOnCloseChecked(bool checked)
{
	hideOnCloseAction->setChecked(checked);
}

void TrayIcon::setAlwaysOnTopState(bool checked)
{
	alwaysOnTopAction->setChecked(checked);
}

bool TrayIcon::getAlwaysOnTopState()
{
	return alwaysOnTopAction->isChecked();
}

int TrayIcon::getVisibleElementsAmount()
{
	return visibleElementsAmount;
}

void TrayIcon::setVisibleElementsAmount(int amount)
{
	if(amount > 99 || amount < 0)
		return;
	changeVisibleElementsAmount(amount);
}

void TrayIcon::changeVisibleElementsAmount(int amount)
{
	if(amount > visibleElementsAmount)
	{
		int bottomRow = model->rowCount();
		if(bottomRow > amount)
			bottomRow = amount;
		
		int lastElement = model->rowCount();
		if(lastElement > visibleElementsAmount)
			lastElement = visibleElementsAmount;

		QList< QAction * > actionList;

		for(int i = visibleElementsAmount; i < bottomRow; i++)
		{
			QMenu *entryMenu = new QMenu( model->data( model->index(i, 0) ).toString(), menu);

			QAction *action = new QAction(tr("Copy username"), entryMenu);
			action->setData( model->data( model->index(i, 2) ) );
			entryMenu->addAction(action);

			action = new QAction(tr("Copy password"), entryMenu);
			action->setData( model->data( model->index(i, 3) ) );
			entryMenu->addAction(action);
			
			actionList.append(entryMenu->menuAction());
		}

		menu->insertActions( menu->actions()[lastElement], actionList);
		visibleElementsAmount = amount;
	}
	else if(amount < visibleElementsAmount)
	{
		if(model->rowCount() > amount)
		{
			int bottomRow = model->rowCount();
			if(bottomRow > visibleElementsAmount)
				bottomRow = visibleElementsAmount;

			QList< QAction * > actionList = menu->actions();
			for(int i = amount; i < bottomRow; i++)
			{
				delete actionList[i];
			}
		}
		visibleElementsAmount = amount;
	}
}

void TrayIcon::handleActivated(QSystemTrayIcon::ActivationReason reason)
{
	if(reason == QSystemTrayIcon::Trigger)
		emit clicked();
}

void TrayIcon::changeData(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
	QList< QAction* > list = menu->actions();

	int left = topLeft.column();
	int right = bottomRight.column();

	int bottomRow = bottomRight.row();
	if(bottomRow > visibleElementsAmount-1)
		bottomRow = visibleElementsAmount-1;

	for(int i = topLeft.row(); i <= bottomRow; i++)
	{
		if(left == 0)
			list[i]->setText( model->data( model->index(i, 0) ).toString() );
		if(left <= 2 && right >= 2)
		{
			QList< QAction* > subList = list[i]->menu()->actions();
			subList[0]->setData( model->data( model->index(i, 2) ) );
		}
		if(left <= 3 && right >= 3)
		{
			QList< QAction* > subList = list[i]->menu()->actions();
			subList[1]->setData( model->data( model->index(i, 3) ) );
		}
	}
}

void TrayIcon::insertRows(const QModelIndex &parent, int start, int end)
{
	QList< QAction * > actionList;
	
	int bottomRow = end; 
	if(bottomRow > visibleElementsAmount-1)
		bottomRow = visibleElementsAmount-1;

	for(int i = start; i <= bottomRow; i++)
	{
		QMenu *entryMenu = new QMenu( model->data( model->index(i, 0) ).toString(), menu);

		QAction *action = new QAction( tr("Copy username"), entryMenu);
		action->setData( model->data( model->index(i, 2) ) );
		entryMenu->addAction(action);

		action = new QAction( tr("Copy password"), entryMenu);
		action->setData( model->data( model->index(i, 3) ) );
		entryMenu->addAction(action);

		actionList.append( entryMenu->menuAction() );
	}
	if(start <= bottomRow)
		menu->insertActions( menu->actions()[start], actionList);
}

void TrayIcon::removeRows(const QModelIndex &parent, int start, int end)
{
	QList< QAction * > actionList = menu->actions();

	int bottomRow = end; 
	if(bottomRow > visibleElementsAmount-1)
		bottomRow = visibleElementsAmount-1;
	
	for(int i = start; i <= bottomRow; i++)
	{
		delete actionList[i];
	}
}

void TrayIcon::copyDataToClipboard(QAction *action)
{
	QApplication::clipboard()->setText( action->data().toString() );
}

