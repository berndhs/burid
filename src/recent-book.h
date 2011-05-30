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

namespace burid
{
class RecentBook
{
public:

  RecentBook (const QString & title = QString(), 
              const QString & author = QString(), 
                    qint64 lastSeen = 0,
              const QString & filename = QString())
    :mTitle (title),
     mAuthor (author),
     mLastSeen (lastSeen),
     mFilename (filename)
  {}

  RecentBook (const RecentBook & other)
    :mTitle (other.mTitle),
     mAuthor (other.mAuthor),
     mLastSeen (other.mLastSeen),
     mFilename (other.mFilename)
  {}

  QString title () const;
  QString author () const;
  qint64  lastSeen () const;
  QString filename () const;

  void setTitle (const QString & title);
  void setAuthor (const QString & author);
  void setLastSeen (qint64 lastSeen);
  void setFilename (const QString & filename);

private:

  QString   mTitle;
  QString   mAuthor;
  qint64    mLastSeen;
  QString   mFilename;

};


typedef QList <RecentBook>  RecentBookList;

} // namespace

#endif
