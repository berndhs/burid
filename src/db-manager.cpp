
#include "db-manager.h"

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
#include <QDesktopServices>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QVariant>
#include <QDebug>
#include "deliberate.h"

using namespace deliberate;

namespace burid
{

DBManager::DBManager (QObject *parent)
  :QObject (parent),
   dbRunning (false)
{
}

DBManager::~DBManager ()
{
  if (dbRunning) {
    Stop ();
  }
}


void
DBManager::Start ()
{
  QString dataDir = QDesktopServices::storageLocation
                    (QDesktopServices::DataLocation);
  QString libraryBasename = dataDir + QDir::separator() + QString ("library.sql");
  libraryBasename = Settings().value ("database/library",libraryBasename)
                              .toString();
  Settings().setValue ("database/library",libraryBasename);
  

  StartDB (bookDB, "libraryBaseCon", libraryBasename);

  QStringList  libraryElements;
  libraryElements << "bookmarks"
                  << "recentbooks";

  CheckDBComplete (bookDB, libraryElements);
 
  emit Started ();
}

void
DBManager::Stop ()
{
  if (dbRunning) {
    dbRunning = false;
    bookDB.close ();
  }
}

void
DBManager::StartDB (QSqlDatabase & db,
                    const QString & conName, 
                    const QString & dbFilename)
{
  db = QSqlDatabase::addDatabase ("QSQLITE", conName);
  CheckFileExists (dbFilename);
  db.setDatabaseName (dbFilename);
  bool ok = db.open ();
  qDebug () << " open db " << ok 
          << " dbname " << db.databaseName ()
          << " file " << dbFilename;
}

void
DBManager::CheckFileExists (const QString & filename)
{
  QFileInfo info (filename);
  if (!info.exists ()) {
    QDir  dir (info.absolutePath ());
    dir.mkpath (info.absolutePath ());
    QFile  file (filename);
    file.open (QFile::ReadWrite);
    file.write (QByteArray (""));
    file.close();
  }
}

void
DBManager::CheckDBComplete (QSqlDatabase & db,
                            const QStringList & elements)
{
  QString eltName, kind;
  qDebug () << " checking DB for elements " << elements;
  for (int e=0; e<elements.size(); e++) {
    eltName = elements.at(e);
    kind = ElementType (db, eltName).toUpper();
    qDebug () << " element " << eltName << " is kind " << kind;
    if (kind != "TABLE" && kind != "INDEX") {
      MakeElement (db, eltName);
    }
  }
}

QString
DBManager::ElementType (QSqlDatabase & db, const QString & name)
{
  QSqlQuery query (db);
  QString pat ("select * from main.sqlite_master where name=\"%1\"");
  QString cmd = pat.arg (name);
  bool ok = query.exec (cmd);
  if (ok && query.next()) {
    QString tipo = query.value(0).toString();
    return tipo;
  }
  return QString ("none");
}

bool
DBManager::Write (const Bookmark & bookmark)
{
  QSqlQuery insert (bookDB);
  insert.prepare ("insert or replace into bookmarks "
                  " (bookfile, markname, spinetitle, pageoffset, scale) "
                  " values (?, ?, ?, ?, ?)");
  insert.bindValue (0,QVariant (bookmark.bookFile ()));
  insert.bindValue (1,QVariant (bookmark.markName ()));
  insert.bindValue (2,QVariant (bookmark.spineItem ()));
  insert.bindValue (3,QVariant (bookmark.stringOffset ()));
  insert.bindValue (4,QVariant (bookmark.stringScale ()));
  bool ok = insert.exec ();
  qDebug () << " bookmark insert " << ok << insert.executedQuery();
  return ok;
}

bool
DBManager::Write (const RecentBook & book)
{
  QSqlQuery insert (bookDB);
  insert.prepare ("insert or replace into recentbooks "
                  " (title, author, lastseen, filename) "
                  " values (?, ?, ?, ?)");
  insert.bindValue (0,QVariant (book.title ()));
  insert.bindValue (1,QVariant (book.author ()));
  insert.bindValue (2,QVariant (book.lastSeen ()));
  insert.bindValue (3,QVariant (book.filename ()));
  bool ok = insert.exec ();
  qDebug () << " bookmark insert " << ok << insert.executedQuery();
  return ok;
}

bool
DBManager::Remove (const Bookmark & bookmark)
{
  QSqlQuery remove (bookDB);
  QString pat ("delete from bookmarks where "
                   "bookfile = \"%1\" AND "
                   "markname = \"%2\" AND "
                   "spinetitle = \"%3\" AND "
                   "pageoffset = \"%4\" AND "
                   "scale = \"%5\" ");
  bool ok = remove.exec (pat.arg (bookmark.bookFile())
                            .arg (bookmark.markName())
                            .arg (bookmark.spineItem())
                            .arg (bookmark.stringOffset())
                            .arg (bookmark.stringScale()) );
  qDebug () << __PRETTY_FUNCTION__ 
            << " bookmark remove " << ok << remove.executedQuery();
  return ok;
}

bool
DBManager::Remove (const RecentBook & book)
{
  QSqlQuery remove (bookDB);
  QString pat ("delete from recentbooks where "
                   "title = \"%1\" AND "
                   "author = \"%2\" AND "
                   "filename = \"%3\" ");
  bool ok = remove.exec (pat.arg (book.title())
                            .arg (book.author())
                            .arg (book.filename()) );
  qDebug () << __PRETTY_FUNCTION__ 
            << " recentbook remove " << ok << remove.executedQuery();
  return ok;
}

bool
DBManager::ReadAll (const QString & bookfile,
                       BookmarkList & list)
{
  QSqlQuery select (bookDB);
  QString cmd (QString ("select markname, spinetitle, pageoffset, scale "
                        " from bookmarks "
                        " where bookfile = \"%1\"")
               .arg (bookfile));
  bool ok = select.exec (cmd);
  bool good (false);
  list.clear ();
  while (ok && select.next ()) {
    Bookmark mark (bookfile,
                   select.value(0).toString (),
                   select.value(1).toString (),
                   select.value(2).toDouble (),
                   select.value(3).toDouble ());
    list.append (mark);
    good = true;
  }
  return good;
}

bool
DBManager::ReadAll (RecentBookList & list)
{
  QSqlQuery select (bookDB);
  QString cmd ("select title, author, lastseen, filename "
                        " from recentbooks "
                        " ORDER BY lastseen DESC");
  bool ok = select.exec (cmd);
  bool good (false);
  list.clear ();
  while (ok && select.next ()) {
    RecentBook book (select.value(0).toString (),
                   select.value(1).toString (),
                   select.value(2).toLongLong (),
                   select.value(3).toString ());
    list.append (book);
    good = true;
  }
  return good;
}



void
DBManager::MakeElement (QSqlDatabase & db, const QString & element)
{
  QString filename (QString (":/schemas/%1.sql").arg (element));
  QFile  schemafile (filename);
  schemafile.open (QFile::ReadOnly);
  QByteArray  createcommands = schemafile.readAll ();
  schemafile.close ();
  QString cmd (createcommands);
  QSqlQuery query (db);
  query.prepare (cmd);
  bool ok = query.exec ();
qDebug () << " tried " << ok << " to create element with " 
          << query.executedQuery ();
}



} // namespace

