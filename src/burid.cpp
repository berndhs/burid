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

namespace burid
{

Burid::Burid (QWidget *parent)
  :QDeclarativeView (parent),
   app (0),
   qmlRoot (0)
{
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
  qmlRoot = rootObject();
  if (qmlRoot) {
    qmlRoot->setProperty ("appTitle",QString (
             "<b>BuRid</b><br>"
              "Book Reader<br>"
             "<i>by Bernd Stramm</i>"));
    connect (qmlRoot, SIGNAL (quitApp()), this, SLOT (Quit()));
  }
  show ();
  qDebug () << __PRETTY_FUNCTION__ << " docs location "
            << QDesktopServices::storageLocation (QDesktopServices::DocumentsLocation);
  qDebug () << __PRETTY_FUNCTION__ << qmlRoot->objectName();
}

void
Burid::Quit ()
{
  if (app) {
    app->quit ();
  }
}



} // namespace
