#include "bread.h"

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

namespace bread
{

Bread::Bread (QWidget *parent)
  :QDeclarativeView (parent),
   app (0),
   qmlRoot (0)
{
}

void
Bread::Init (QApplication & qapp)
{
  app = &qapp;
}

void
Bread::AddConfigMessages (const QStringList & messages)
{
  configMessages.append (messages);
}

void
Bread::Run ()
{
  setSource (QUrl::fromLocalFile ("qml/DefaultMain.qml"));
  qmlRoot = rootObject();
  if (qmlRoot) {
    qmlRoot->setProperty ("appText",QString ("BRead Book Reader"));
  }
  show ();
}



} // namespace

