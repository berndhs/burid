#ifndef BURID_RECENT_MODEL_H
#define BURID_RECENT_MODEL_H


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
#include "recent-book.h"

namespace burid
{

class RecentBookModel : public QAbstractListModel
{
Q_OBJECT

public:

  RecentBookModel (QObject *parent=0);

  int rowCount (const QModelIndex & index = QModelIndex()) const;
  QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;

  RecentBook recentBook (int row) const;

  void setList (const RecentBookList & list);
  void removeRow (int row);

  void appendBook (const RecentBook & book);

  bool haveRow (int row) const { return (0 <= row && row <= rowCount()); }

private:

  enum Data_Type {
    Type_Title = Qt::UserRole+1,
    Type_Author = Qt::UserRole+2,
    Type_LastSeen = Qt::UserRole+3,
    Type_Filename = Qt::UserRole+4
  };

  RecentBookList    dataList;

};

} // namespace

#endif