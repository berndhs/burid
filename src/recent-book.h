#ifndef BURID_RECENT_BOOK_H
#define BURID_RECENT_BOOK_H


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

#include <QString>
#include <QList>
#include "deliberate.h"

using namespace deliberate;

namespace burid
{
class RecentBook
{
public:

  RecentBook (const QString & title = QString(), 
              const QString & author = QString(), 
                    qint64 lastSeen = 0,
              const QString & filename = QString())
    :title (title),
     author (author),
     lastSeen (lastSeen),
     filename (filename)
  {}

  RecentBook (const RecentBook & other)
    :title (other.title()),
     author (other.author()),
     lastSeen (other.lastSeen()),
     filename (other.filename())
  {}

  Property <QString>   title;
  Property <QString>   author;
  Property <qint64>    lastSeen;
  Property <QString>   filename;

};


typedef QList <RecentBook>  RecentBookList;

} // namespace

#endif
