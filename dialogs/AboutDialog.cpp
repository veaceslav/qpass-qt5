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

#include "AboutDialog.h"

#include <QList>
#include <QStringList>
#include <QMessageBox>

#include "qpass-config.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent)
{
   setupUi(this);
   QList< QStringList > authorList;
   
   authorList.append( QStringList() );
   authorList.last() << QString::fromUtf8("Mateusz Piękos") << "mateuszpiekos@gmail.com" << "Main developer";

   QString text;
   text = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">\
<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\
p, li { white-space: pre-wrap; }\
</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">";
   
   QList< QStringList >::iterator i;
   
   for(i = authorList.begin(); i != authorList.end(); ++i)
   {
      text += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">";
      text += i->at(0);
      text += "</p>";
      QStringList::iterator j;
      for(j = i->begin()+1; j != i->end(); ++j)
      {
	 text += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:20px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">";
	 text += *j;
	 text += "</p>";
      }
      text += "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"> </p>";
   }
   
   text += "</body></html>";
   
   authorsLabel->setText(text);
   
   QString topLabelText;
   topLabelText = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\
<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\
p, li { white-space: pre-wrap; }\
</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\
<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:16pt;\">QPass</span></p>\
<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">";
   topLabelText += tr( "version " );
   topLabelText += VERSION;
   topLabelText += "</span></p>\
<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>";
   topLabel->setText(topLabelText);
}
