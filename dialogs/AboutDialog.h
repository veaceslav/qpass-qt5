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

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

#include "ui_AboutDialog.h"

class AboutDialog : public QDialog, private Ui::AboutDialog
{
   Q_OBJECT
   public:
      AboutDialog(QWidget *parent);
};

#endif //ABOUTDIALOG_H