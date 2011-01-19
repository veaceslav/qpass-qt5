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

#include "NewDatabaseDialog.h"

#include <QMessageBox>

NewDatabaseDialog::NewDatabaseDialog(QWidget *parent) : QDialog(parent)
{
   welcomeWidget = new QWidget(this);
   uiWelcome.setupUi(welcomeWidget);
   passwordWidget = new QWidget(this);
   uiPassword.setupUi(passwordWidget);
   
   stackedWidget = new QStackedWidget(this);
   stackedWidget->addWidget(welcomeWidget);
   stackedWidget->addWidget(passwordWidget);
   
   QVBoxLayout *layout = new QVBoxLayout(this);
   layout->addWidget(stackedWidget);
   this->setLayout(layout);
   
   connect(uiWelcome.newButton, SIGNAL(clicked()), this, SLOT(next()));
   connect(uiPassword.buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
   connect(uiPassword.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

QString NewDatabaseDialog::value()
{
   return uiPassword.passwordEdit->text();
}

void NewDatabaseDialog::next()
{
   if(stackedWidget->currentWidget() == welcomeWidget)
      stackedWidget->setCurrentWidget(passwordWidget);
}

void NewDatabaseDialog::accept()
{
   if(uiPassword.passwordEdit->text() != uiPassword.passwordEdit2->text())
   {
      QMessageBox box(this);
      box.setText( tr("Passwords does not match") );
      box.setIcon( QMessageBox::Warning );
      box.exec();
   }
   else
      done(QDialog::Accepted);
}