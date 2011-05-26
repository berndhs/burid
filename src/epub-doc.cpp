#include "epub-doc.h"


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

#include <QFile>
#include <QDomDocument>
#include <QDesktopServices>
#include <QDir>
#include <QProcess>
#include <QUuid>
#include <QUrl>
#include <QDebug>
#include "burid-magic.h"

namespace burid
{
EpubDoc::EpubDoc (QObject *parent)
  :QObject (parent)
{
}

QString
EpubDoc::nextItem (int offset)
{
  qDebug () << __PRETTY_FUNCTION__ << offset;
  currentSpineItem += offset;
  if (0 <= currentSpineItem  && currentSpineItem < spine.count()) {
    QUrl nextUrl ( currentDir + QDir::separator() 
                      + manifest [spine.at(currentSpineItem)].href);
    if (nextUrl.scheme() == "") {
      nextUrl.setScheme ("file");
    }
    return nextUrl.toString();
  } 
  return QString();
}

QString
EpubDoc::startItem ()
{
  qDebug () << __PRETTY_FUNCTION__;
  currentSpineItem = 0;
  if (spine.count() > 0) {
    QUrl startUrl ( currentDir + QDir::separator() 
                      + manifest [spine.at(0)].href);
    if (startUrl.scheme() == "") {
      startUrl.setScheme ("file");
    }
    return startUrl.toString();
  } else {
    return QString();
  }
}

void
EpubDoc::openBook (const QString & filename)
{
  manifest.clear ();
  spine.clear ();
  QString tmpname;
  unzip (filename, tmpname);
  if (tmpname.isEmpty()) {
    return;
  }
  QFile  infile (tmpname);
  bool ok = infile.open (QFile::ReadOnly);
  if (ok) {
    QDomDocument  doc;
    qDebug () << __PRETTY_FUNCTION__ << " before setContent";
    doc.setContent (&infile);
    //qDebug () << __PRETTY_FUNCTION__ << doc.toString(1);
    ReadManifests (doc.elementsByTagName ("manifest"));
    ReadSpines (doc.elementsByTagName ("spine"));
  }
  qDebug () << __PRETTY_FUNCTION__ << " file " << tmpname << ok;
  qDebug () << "   Spine: " << spine;
  QString startUrl = startItem();
  qDebug () << "    want them to read " << startUrl;
  emit startBook (startUrl);
}

void
EpubDoc::ReadManifests (const QDomNodeList & manifests)
{
  for (int i=0; i<manifests.count(); i++) {
    QDomNodeList itemList  = manifests.at(i).toElement()
                              .elementsByTagName("item");
    for (int j=0; j<itemList.count(); j++) {
      QDomElement elt = itemList.at(j).toElement();
      QString href = elt.attribute("href");
      QString id = elt.attribute("id");
      QString mediaType = elt.attribute("media-type");
      if (href.length() > 0 && id.length() > 0) {
        manifest[id] = ManifestRec (id,href,mediaType);
      }
    }
  }
}

void
EpubDoc::ReadSpines (const QDomNodeList & spines)
{
  for (int i=0; i<spines.count(); i++) {
    QDomNodeList oneSpine = spines.at(i).toElement()
                             .elementsByTagName ("itemref");
    for (int j=0; j<oneSpine.count(); j++) {
      QDomElement elt = oneSpine.at(j).toElement();
      QString idref = elt.attribute ("idref");
      if (!idref.isEmpty()) {
        spine.append (idref);
      }
    }
  }
}

void
EpubDoc::unzip (const QString & compressedName, QString & clearName)
{
  QString tmpRoot = QDesktopServices::storageLocation 
                      (QDesktopServices::TempLocation);
  QString tmpName (tmpRoot + QDir::separator() 
                    + QString("burid") + QDir::separator()
                    + "unpack_");
  tmpName.append (QUuid::createUuid().toString().remove(QRegExp("[{}-]")));
  QDir tmpRootDir (tmpRoot);
  tmpRootDir.mkpath (tmpName);
  tempDirs.append (tmpName);
  currentDir = tmpName;
  QProcess::execute (Magic::UnzipProgram
                     + QString (" ")
                     + compressedName
                     + QString (" -d ")
                     + tmpName);
  QStringList filter;
  QDir unpackDir (tmpName);
  filter.append ("*.opf");
  QStringList files = unpackDir.entryList (filter, 
                   QDir::Files | QDir::NoSymLinks | QDir::Readable);
  if (files.count() == 1) {
    clearName = tmpName + QDir::separator() + files.at(0);
  } else {
    clearName.clear();
  }
  qDebug () << __PRETTY_FUNCTION__ << "  tmp unzipped should be " << clearName;
}

void
EpubDoc::clearCache ()
{
}

EpubDoc::ManifestRec::ManifestRec ()
{
}

EpubDoc::ManifestRec::ManifestRec (const QString & theId, 
                   const QString & theHref, 
                   const QString & theMedia)
  :id (theId),
   href (theHref),
   mediaType (theMedia)
{
}

EpubDoc::ManifestRec::ManifestRec (const ManifestRec & other)
  :id (other.id),
   href (other.href),
   mediaType (other.mediaType)
{
}

} // namespace