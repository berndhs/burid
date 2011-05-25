#include "burid.h"

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

#include <QUrl>
#include <QDebug>
#include <QDesktopServices>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include "deliberate.h"

using namespace deliberate;

namespace burid
{

Burid::Burid (QWidget *parent)
  :QDeclarativeView (parent),
   app (0),
   qmlRoot (0),
   //epubDoc (this),
   pdfPager (0),
   saveTimer (this)
{
  setResizeMode (QDeclarativeView::SizeRootObjectToView);
  pdfPager = new PdfPager (0);
  QSize curSize = Settings ().value ("+size", size()).toSize();
  qDebug () << " want size " << curSize;
  resize (curSize);
  qDebug () << " ------------- forced size " << size();
  connect (&saveTimer, SIGNAL (timeout()), this, SLOT (periodicSave()));
  saveTimer.start (60*1000); // minute
  QTimer::singleShot (3*1000, this, SLOT (periodicSave()));
}

Burid::~Burid ()
{
  qDebug () << __PRETTY_FUNCTION__ << " That's all folks";
  if (qmlRoot) {
    disconnect (qmlRoot, 0,0,0);
  }
  delete pdfPager;
}

void
Burid::resizeEvent (QResizeEvent * event)
{
  qDebug () << "Resize to " << event->size();
  QDeclarativeView::resizeEvent (event);
}

void
Burid::periodicSave ()
{
  Settings().setValue ("+size",size());
  Settings().sync();
  qDebug () << __PRETTY_FUNCTION__;
}

void
Burid::Init (QApplication & qapp)
{
  app = &qapp;
}

void
Burid::AddConfigMessages (const QStringList & messages)
{
  configMessages.append (messages);
}

void
Burid::Run ()
{
  setSource (QUrl("qrc:/DefaultMain.qml"));
  setResizeMode (QDeclarativeView::SizeRootObjectToView);
  QSize curSize = Settings ().value ("+size", size()).toSize();
  qDebug () << " want size " << curSize;
  resize (curSize);
  qmlRoot = rootObject();
  if (qmlRoot) {
    qmlRoot->setProperty ("appTitle",QString (
             "<b>BuRid</b><br>"
              "Book Reader<br><br>"
             "by <i>Bernd Stramm</i>"));
    connect (qmlRoot, SIGNAL (quitApp()), this, SLOT (Quit()));
    connect (qmlRoot, SIGNAL (startReadPdf()), this, SLOT (startPdf()));
  }
  show ();
  qDebug () << __PRETTY_FUNCTION__ << " docs location "
            << QDesktopServices::storageLocation (QDesktopServices::DocumentsLocation);
  qDebug () << __PRETTY_FUNCTION__ << qmlRoot->objectName();
  QObject * pdfImage = qmlRoot->findChild<QObject*> ("PdfPageImage");
  qDebug () << "            " << pdfImage;
  QDeclarativeEngine * dengine = engine();
  qDebug () << __PRETTY_FUNCTION__ << " engine " << dengine;
  if (dengine) {
    dengine->addImageProvider(QString("pdfpager"),pdfPager);
    qDebug () << "   image providers " << dengine->imageProvider (QString("pdfpager"));
    qDebug () << "   pdf pager       " << pdfPager;
  }
  QDeclarativeContext * dcontext = rootContext();
  if (dcontext) {
    dcontext->setContextProperty ("pdfPagerIF",pdfPager);
    //dcontext->setContextProperty ("epubDocIF",&epubDoc);
  }
}

void
Burid::startPdf ()
{
  qDebug () << __PRETTY_FUNCTION__;
  pdfPager->LoadPDF ("data/pdf/control_arxiv.pdf");
  QStringList keys = pdfPager->infoKeys ();
  for (int i=0; i< keys.count(); i++) {
    qDebug () << "    doc " << keys.at(i) << pdfPager->info (keys.at(i));
  }
}

void
Burid::startEpub ()
{
}

void
Burid::Quit ()
{
  Settings().setValue ("+size",size());
  Settings().sync();
  if (app) {
    app->quit ();
  }
  qDebug () << __PRETTY_FUNCTION__ << " All Done ";
}



} // namespace

