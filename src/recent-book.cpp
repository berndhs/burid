#include "recent-book.h"


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


namespace burid
{
QString
RecentBook::title () const
{
  return mTitle;
}

QString 
RecentBook::author () const
{
  return mAuthor;
}

qint64
RecentBook::lastSeen () const
{
  return mLastSeen;
}

QString 
RecentBook::filename () const
{
  return mFilename;
}

void
RecentBook::setTitle (const QString & title)
{
  mTitle = title;
}

void
RecentBook::setAuthor (const QString & author)
{
  mAuthor = author;
}

void
RecentBook::setLastSeen (qint64 lastSeen)
{
  mLastSeen = lastSeen;
}


void
RecentBook::setFilename (const QString & filename)
{
  mAuthor = filename;
}

} // namespace
