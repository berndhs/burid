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

namespace burid
{

class EpubDoc: public QObject 
{
Q_OBJECT

public:

  EpubDoc (QObject *parent=0);
  Q_INVOKABLE QString nextItem (int offset);
  Q_INVOKABLE QString startItem ();
  Q_INVOKABLE void    mark (qreal pageY, qreal pageScale);

  void openBook (const QString & filename);
  void clearCache ();

signals:

  void startBook (const QString & startUrl);

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

  void unzip (const QString & compressedName, QString & clearName);
  void ReadMeta (const QDomNodeList & manifests);
  void ReadManifests (const QDomNodeList & manifests);
  void ReadSpines (const QDomNodeList & spines);
  void CollectTexts (const QDomNodeList & nodeList,
                       QStringList & values);

  QString                        currentDir;
  int                            currentSpineItem;
  QString                        currentAuthor;
  QString                        currentTitle;
  QStringList                    currentSubjects;
  QString                        origBookFile;
  QMap <QString, ManifestRec>    manifest;    // map of book part ids to files
  QList <QString>                spine;       // linear order of book parts

  QStringList    tempDirs;

}; 

} // namespace

#endif
