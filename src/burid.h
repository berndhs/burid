#ifndef BURID_BURID_H
#define BURID_BURID_H


/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2011, Bernd Stramm
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/


#include <QDeclarativeView>
#include <QStringList>
#include <QApplication>
#include <QGraphicsObject>
#include <QTimer>
#include <QResizeEvent>
#include "epub-doc.h"
#include "pdf-pager.h"
#include "db-manager.h"

namespace burid
{
class Burid: public QDeclarativeView
{
Q_OBJECT
public:

  Burid (QWidget *parent=0);
  ~Burid ();

  void Init (QApplication & qapp,
             const QStringList & fileList = QStringList(),
             const QStringList & confMessages = QStringList());
  void Run ();

public slots:

  void Quit ();

private slots:

  void startPdf (const QString & file = QString());
  void startEpub (const QString & file = QString());

  void startReadEpub (const QString & startUrl);
  void jumpToEpub (const QString & url, qreal offset, qreal scale);

  void periodicSave ();

  void doneReading ();


protected:

  void resizeEvent (QResizeEvent * event);

private:

  QApplication     *app;
  QStringList       fileList;
  QStringList       configMessages;
  QGraphicsObject  *qmlRoot;

  DBManager             dbm;
  EpubDoc               epubDoc;
  PdfPager             *pdfPager;
  
  QTimer               saveTimer;
};

} // namespace

#endif
