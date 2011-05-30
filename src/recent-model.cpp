#include "recent-model.h"


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

RecentBookModel::RecentBookModel (QObject *parent)
  :QAbstractListModel (parent)
{
  QHash<int, QByteArray> roles;
  roles[Type_Title] = "title";
  roles[Type_Author] = "author";
  roles[Type_LastSeen] = "lastSeen";
  roles[Type_Filename] = "filename";
  setRoleNames(roles);
}

int
RecentBookModel::rowCount (const QModelIndex & index) const
{
  Q_UNUSED (index)
  return dataList.count();
}

QVariant
RecentBookModel::data (const QModelIndex & index, int role) const
{
  if (!index.isValid()) {
    return QVariant();
  }
  int row = index.row();
  QVariant retVar;
  switch (role) {
  case Qt::DisplayRole:
  case Type_Title:
    retVar = dataList.at(row).title();
    break;
  case Type_Author:
    retVar = dataList.at(row).author();
    break;
  case Type_LastSeen:
    retVar = dataList.at(row).lastSeen();
    break;
  case Type_Filename:
    retVar = dataList.at(row).filename();
    break;
  default:
    break;
  }
  return retVar;
}

RecentBook 
RecentBookModel::recentBook (int row) const
{
  return dataList.at (row);
}

void
RecentBookModel::setList (const RecentBookList & list)
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
RecentBookModel::appendBook (const RecentBook & book)
{
  int nr = rowCount();
  beginInsertRows (QModelIndex(), nr, nr);
  dataList.append (book);
  endInsertRows ();
}

void
RecentBookModel::removeRow (int row)
{
  int nr = rowCount();
  if (0 <= row && row < nr) {
    beginRemoveRows (QModelIndex(), row, row);
    dataList.removeAt (row);
    endRemoveRows ();
  }
}

} // namespace
