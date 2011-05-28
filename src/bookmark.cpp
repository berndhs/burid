#include "bookmark.h"


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

Bookmark::Bookmark ()
  :mOffset (0.0)
{
}

Bookmark::Bookmark (const QString & bookfile,
            const QString & markname,
            const QString & spineitem,
                  double  offset,
                  double  scale)
  :mBookFile (bookfile),
   mMarkName (markname),
   mSpineItem (spineitem),
   mOffset (offset),
   mScale (scale)
{
}

Bookmark::Bookmark (const Bookmark & other)
  :mBookFile (other.mBookFile),
   mMarkName (other.mMarkName),
   mSpineItem (other.mSpineItem),
   mOffset (other.mOffset),
   mScale (other.mScale)
{
}

QString
Bookmark::stringOffset (char format, int precision) const
{
  return QString::number (mOffset, format, precision);
}

QString
Bookmark::stringScale (char format, int precision) const
{
 return QString::number (mScale, format, precision);
}


QDebug 
operator<< (QDebug dbg, const Bookmark & bm)
{
  dbg.nospace() << "( Bookmark "
                << " file " << bm.bookFile ()
                << " mark " << bm.markName ()
                << " spine " << bm.spineItem ()
                << " offset " << bm.offset ()
                << " scale " << bm.scale ()
                << ")";
  return dbg.space();
}

} // namespace
