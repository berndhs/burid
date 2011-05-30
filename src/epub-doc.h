#ifndef BURID_EPUB_DOC_H
#define BURID_EPUB_DOC_H


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


#include <QObject>
#include <QString>
#include <QStringList>
#include <QDomNodeList>
#include <QList>
#include <QMap>
#include <QProcess>
#include "db-manager.h"
#include "bookmark-model.h"
#include "recent-model.h"

namespace burid
{

class EpubDoc: public QObject 
{
Q_OBJECT

public:

  EpubDoc (QObject *parent, DBManager & dbmanager);
  Q_INVOKABLE QString nextItem (int offset);
  Q_INVOKABLE QString startItem ();
  Q_INVOKABLE void    mark (const QString & markText, 
                            qreal pageY, 
                            qreal pageScale);
  Q_INVOKABLE void    removeMark (int row);
  Q_INVOKABLE QString nextBookmark();

  Q_INVOKABLE BookmarkModel * bookmarkModel ();
  Q_INVOKABLE RecentBookModel * recentBookModel ();

  Q_INVOKABLE int markRowCount();

  Q_INVOKABLE void jumpToBookmark (int index);
  Q_INVOKABLE void openRecentBook (int index);
  Q_INVOKABLE void forgetRecentBook (int index);

  Q_PROPERTY(QString contentPart READ contentPart NOTIFY partChanged);
  QString contentPart ();
  
  void openBook (const QString & filename);
  void clearCache ();

private slots:

  void unpackStarted ();
  void unpackError (QProcess::ProcessError err);
  void unpackDone (int exitCode, QProcess::ExitStatus exitStatus);

  void resetDB ();

signals:

  void startBook (const QString & startUrl);
  void jumpIntoBook (const QString & url, qreal offset, qreal scale);
  void marksChanged (int markCount);
  void partChanged (const QString & contentPart);

private:

  class ManifestRec {
    public:
      ManifestRec ();
      ManifestRec (const QString & theId, 
                   const QString & theHref, 
                   const QString & theMedia);
      ManifestRec (const ManifestRec & other);
      QString    id;
      QString    href;
      QString    mediaType;
  };

  void unzip (const QString & compressedName);
  void ReadMeta (const QDomNodeList & manifests);
  void ReadManifests (const QDomNodeList & manifests);
  void ReadSpines (const QDomNodeList & spines);
  void CollectTexts (const QDomNodeList & nodeList,
                       QStringList & values);

  DBManager                    & dbm;
  BookmarkModel                  markModel;
  RecentBookModel                recentBooks;
  QProcess                       unpacker;
  QString                        unpackTmpName;
  QString                        currentDir;
  int                            currentSpineItem;
  QStringList                    currentAuthors;
  QString                        currentTitle;
  QStringList                    currentSubjects;
  QString                        origBookFile;
  int                            maxRecentBooks;
  QMap <QString, ManifestRec>    manifest;    // map of book part ids to files
  QList <QString>                spine;       // linear order of book parts


  QStringList    tempDirs;

}; 

} // namespace

#endif
