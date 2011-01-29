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

#ifndef PASSWORDGENERATORDIALOG_H
#define PASSWORDGENERATORDIALOG_H

#define IMAGE_WIDTH 255
#define IMAGE_HEIGHT 192

#include <QDialog>
#include <QStackedWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QProgressBar>
#include <QGraphicsView>

#include "ui_PasswordGeneratorDialogPage1.h"
#include "ui_PasswordGeneratorDialogPage3.h"

class GraphicGenerator;

enum characterSetValues
{
   smallCharacters = 1,
   bigCharacters = 2,
   digits = 4,
   specialCharacters = 8
};

/*!
 * This class provides a password generator.
 */
class PasswordGeneratorDialog : public QDialog
{
   Q_OBJECT
   public:
      PasswordGeneratorDialog(QWidget *parent);
      /*! 
       * Function returns generated value.
       */
      QString getResult();
   private:
      Ui::PasswordGeneratorDialogPage1 uiSettings;
      Ui::PasswordGeneratorDialogPage3 uiResult;
      QStackedWidget *stackedWidget;
      QWidget *settingsWidget;
      QWidget *resultWidget;
      QWidget *generatorWidget;
      QWidget *buttonsWidget;
      QPushButton *nextButton;
      QPushButton *backButton;
      GraphicGenerator *generator;
      QProgressBar *progressBar;
   private slots:
      void back();
      void next();
};

/*!
 * This class provides a generator which calculates random values
 * based on mouse moves.
 */
class GraphicGenerator : public QGraphicsView
{
   Q_OBJECT
   public:
      GraphicGenerator(QProgressBar *progressBar, QWidget *parent);
      /*! This function initializes generator.
       * 
       * This function calculates character set map,
       * calculates pseudo random table and initializes variables.
       * 
       * @param size Size of string to generate.
       * @param characterSet Combination of characterSetValues.
       */
      void initGenerator(int size, int characterSet);
      /*! 
       * Function returns generated value.
       */
      QString getResult();
   protected:
      void mouseMoveEvent( QMouseEvent *event);
   private:
      QProgressBar *progressBar;
      int count;
      int size;
      int value;
      int table[IMAGE_WIDTH][IMAGE_HEIGHT];
      char characterMap[128];
      int characterMapCount;
      QString result;
   signals:
      /*!
       * Signal emitted when generation is complete
       */
      void generatingEnd();
};

#endif //PASSWORDGENERATORDIALOG_H
