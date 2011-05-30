#ifndef DB_MANAGER_H
#define DB_MANAGER_H

/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2010, Bernd Stramm
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
#include <QSqlDatabase>
#include "bookmark.h"
#include "recent-book.h"
#include <QMap>
#include <QObject>

namespace burid
{
class DBManager : public QObject
{
Q_OBJECT

public:

  DBManager (QObject *parent = 0);
  ~DBManager ();

  void  Start ();
  void  Stop ();
  bool  Running () { return dbRunning; }

  bool  Write (const Bookmark &  bookmark);
  bool  Write (const RecentBook & book);

  bool  Remove (const Bookmark & bookmark);
  bool  Remove (const RecentBook & book);

  bool  ReadAll (const QString & bookfile,
                       BookmarkList & list);
  bool  ReadAll (RecentBookList & bookList);

private:

  void StartDB (QSqlDatabase & db,
                    const QString & conName, 
                    const QString & dbFilename);
  void CheckFileExists (const QString & filename);
  void CheckDBComplete (QSqlDatabase & db,
                        const QStringList & elements);
  QString ElementType (QSqlDatabase & db, const QString & name);
  void    MakeElement (QSqlDatabase & db, const QString & element);


  QSqlDatabase     bookDB;
  bool             dbRunning;

};

} // namespace

#endif
