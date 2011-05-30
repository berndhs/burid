#ifndef BURID_BOOKMARK_MODEL_H
#define BURID_BOOKMARK_MODEL_H


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


#include <QAbstractListModel>
#include <QVariant>
#include <QObject>
#include "bookmark.h"

namespace burid
{

class BookmarkModel : public QAbstractListModel
{
Q_OBJECT

public:

  BookmarkModel (QObject *parent=0);

  int rowCount (const QModelIndex & index = QModelIndex()) const;
  QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;

  Bookmark bookmark (int row) const;

  Q_INVOKABLE QString markText (int row);

  void setList (const BookmarkList & list);
  void removeRow (int row);

  void appendMark (const Bookmark & mark);

private:

  enum Data_Type {
    Type_File = Qt::UserRole+1,
    Type_Mark = Qt::UserRole+2,
    Type_Item = Qt::UserRole+3,
    Type_Offset = Qt::UserRole+4,
    Type_Scale = Qt::UserRole+5
  };

  BookmarkList    dataList;

};

} // namespace

#endif