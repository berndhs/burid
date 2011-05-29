#include "bookmark-model.h"


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

#include <QHash>
#include <QByteArray>

namespace burid
{

BookmarkModel::BookmarkModel (QObject *parent)
  :QAbstractListModel (parent)
{
  QHash<int, QByteArray> roles;
  roles[Type_File] = "markFile";
  roles[Type_Mark] = "markTitle";
  roles[Type_Item] = "markItem";
  roles[Type_Offset] = "markOffset";
  roles[Type_Scale] = "markScale";
  setRoleNames(roles);
}

int
BookmarkModel::rowCount (const QModelIndex & index) const
{
  Q_UNUSED (index)
  return dataList.count();
}

QString
BookmarkModel::markText (int row)
{
  return dataList.at(row).markName();
}

QVariant
BookmarkModel::data (const QModelIndex & index, int role) const
{
  if (!index.isValid()) {
    return QVariant();
  }
  int row = index.row();
  QVariant retVar;
  switch (role) {
  case Qt::DisplayRole:
  case Type_Mark:
    retVar = dataList.at(row).markName();
    break;
  case Type_File:
    retVar = dataList.at(row).bookFile();
    break;
  case Type_Item:
    retVar = dataList.at(row).spineItem();
    break;
  case Type_Offset:
    retVar = dataList.at(row).offset();
    break;
  case Type_Scale:
    retVar = dataList.at(row).scale();
    break;
  default:
    break;
  }
  return retVar;
}

Bookmark 
BookmarkModel::bookmark (int row) const
{
  return dataList.at (row);
}

void
BookmarkModel::setList (const BookmarkList & list)
{
  int nr = rowCount();
  if (nr > 0) {
    beginRemoveRows (QModelIndex(), 0, nr+1);
    dataList.clear ();
    endRemoveRows ();
  }
  nr = list.count();
  beginInsertRows (QModelIndex(), 0, nr+1);
  dataList = list;
  endInsertRows ();
}

void
BookmarkModel::appendMark (const Bookmark & mark)
{
  int nr = rowCount();
  beginInsertRows (QModelIndex(), nr, nr);
  dataList.append (mark);
  endInsertRows ();
}

} // namespace
