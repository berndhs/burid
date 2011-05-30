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
#include <QFileInfo>
#include <QDomDocument>
#include <QDesktopServices>
#include <QDir>
#include <QUuid>
#include <QUrl>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>
#include "burid-magic.h"
#include "bookmark.h"
#include "recent-book.h"
#include "deliberate.h"

using namespace deliberate;

namespace burid
{
EpubDoc::EpubDoc (QObject *parent, DBManager & dbmanager)
  :QObject (parent),
   dbm (dbmanager),
   markModel (this),
   unpacker (this),
   currentSpineItem (0),
   maxRecentBooks (10)
{
  connect (&unpacker, SIGNAL (started()), this, SLOT (unpackStarted()));
  connect (&unpacker, SIGNAL (finished(int, QProcess::ExitStatus)), 
            this, SLOT (unpackDone(int, QProcess::ExitStatus)));
  connect (&unpacker, SIGNAL (error(QProcess::ProcessError)), 
            this, SLOT (unpackError(QProcess::ProcessError)));
  maxRecentBooks = Settings ().value ("maxrecentbooks",maxRecentBooks).toInt();
  Settings().setValue ("maxrecentbooks",maxRecentBooks);
}

BookmarkModel *
EpubDoc::bookmarkModel ()
{
  qDebug () << __PRETTY_FUNCTION__ << &markModel;
  return &markModel;
}

void
EpubDoc::jumpToBookmark (int index)
{
  qDebug () << __PRETTY_FUNCTION__ << " jump to row " << index;
  if (0 <= index && index <= markModel.rowCount()) {
    Bookmark  mark = markModel.bookmark (index);
    QString item = mark.spineItem();
    QString href = manifest[item].href;
    double offset = mark.offset ();
    double scale = mark.scale ();
    QUrl nextUrl (currentDir + QDir::separator() + href);
    if (nextUrl.scheme() == "") {
      nextUrl.setScheme ("file");
    }
    emit jumpIntoBook (nextUrl.toString(), offset, scale);
    currentSpineItem = spine.indexOf (item);
    emit partChanged(contentPart());
  }
}

QString
EpubDoc::nextItem (int offset)
{
  qDebug () << __PRETTY_FUNCTION__ << offset;
  currentSpineItem += offset;
  QString retval;
  if (0 <= currentSpineItem  && currentSpineItem < spine.count()) {
    QUrl nextUrl ( currentDir + QDir::separator() 
                      + manifest [spine.at(currentSpineItem)].href);
    if (nextUrl.scheme() == "") {
      nextUrl.setScheme ("file");
    }
    emit partChanged(contentPart());
    retval = nextUrl.toString();
  } 
  //QMessageBox::information (0,QString("Next Page Url"),retval);
  return retval;
}

QString
EpubDoc::startItem ()
{
  qDebug () << __PRETTY_FUNCTION__;
  currentSpineItem = 0;
  emit partChanged(contentPart());
  QString retval;
  if (spine.count() > 0) {
    QUrl startUrl ( currentDir + QDir::separator() 
                      + manifest [spine.at(0)].href);
    if (startUrl.scheme() == "") {
      startUrl.setScheme ("file");
    }
    retval = startUrl.toString();
  } else {
    retval = QString();
  }
  //QMessageBox::information (0,QString("Start Page Url"),retval);
  return retval;
}

QString
EpubDoc::contentPart ()
{
  return tr ("Part %1 of %2")
           .arg (currentSpineItem+1)
           .arg (spine.count());
}


void
EpubDoc::mark (const QString & markText, double pageY, double pageScale)
{
  qDebug () << __PRETTY_FUNCTION__ << origBookFile << markText
            << currentSpineItem << pageY << pageScale;
  Bookmark mark (origBookFile, 
                 markText,
                 spine.at(currentSpineItem),
                 pageY,
                 pageScale);
  dbm.Write (mark);
  markModel.appendMark (mark);
  emit marksChanged (markModel.rowCount());
}

void
EpubDoc::removeMark (int row)
{
  Bookmark mark = markModel.bookmark (row);
  markModel.removeRow (row);
  dbm.Remove (mark);
  emit marksChanged (markModel.rowCount());
}

int
EpubDoc::markRowCount ()
{
  return markModel.rowCount();
}

QString
EpubDoc::nextBookmark ()
{
  static int counter (1);
  return tr ("Bookmark %1").arg(counter++);
}

void
EpubDoc::openBook (const QString & filename)
{
  manifest.clear ();
  spine.clear ();
  origBookFile = filename;
  unzip (filename);
}

void
EpubDoc::unzip (const QString & compressedName)
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
  #if BURID_UNZIP_UNIX
  QStringList args;
  args.append (compressedName);
  args.append ("-d");
  args.append (tmpName);
  unpackTmpName = tmpName;
  unpacker.start (Magic::UnzipProgram, args);
  #else
  QMessageBox::warning (0, QString("No Umcompress!"),
                     tr("Don't know how to Unzip EPUB on this system"));
  #endif
}

void
EpubDoc::unpackStarted ()
{
  qDebug () << __PRETTY_FUNCTION__;
}

void
EpubDoc::unpackError (QProcess::ProcessError err)
{
  qDebug () << __PRETTY_FUNCTION__ << err;
}

void
EpubDoc::unpackDone (int exitCode, QProcess::ExitStatus exitStatus)
{
  qDebug () << __PRETTY_FUNCTION__;
  if (exitCode != 0 || exitStatus != QProcess::NormalExit) {
    qDebug () << " Bad Unpack Process Exit " << exitCode << exitStatus;
    return;
  }
  QString tmpName = unpackTmpName;
  QDir unpackDir (tmpName);
  QFile metaFile (tmpName + QDir::separator() + QString("META-INF")
                  +QDir::separator() + QString("container.xml"));
  bool ok = metaFile.open (QFile::ReadOnly);
  if (!ok) {
    QMessageBox::warning (0, QString ("No Metafile!"),
                      metaFile.fileName());
    return;
  }
  QDomDocument metaDoc;
  metaDoc.setContent (&metaFile);
  QDomNodeList  rootfiles = metaDoc.elementsByTagName ("rootfile");
  QString contentName;
  if (rootfiles.count() >= 1) {
     QString fullPath = rootfiles.at(0).toElement().attribute("full-path");
     if (fullPath.length() > 0) {
       contentName = tmpName + QDir::separator()
                   + fullPath;
       currentDir = QFileInfo (contentName).absolutePath();
     }
  }
  //QMessageBox::information (0, QString("content name"), contentName);
  qDebug () << __PRETTY_FUNCTION__ << "  tmp unzipped should be " << contentName;

  if (contentName.isEmpty()) {
    return;
  }
  QFile  infile (contentName);
  ok = infile.open (QFile::ReadOnly);
  if (ok) {
    QDomDocument  doc;
    qDebug () << __PRETTY_FUNCTION__ << " before setContent";
    doc.setContent (&infile);
    ReadMeta (doc.elementsByTagName ("metadata"));
    ReadManifests (doc.elementsByTagName ("manifest"));
    ReadSpines (doc.elementsByTagName ("spine"));
  }
  QString startUrl = startItem();
  BookmarkList bookmarks;
  dbm.ReadAll (origBookFile, bookmarks);
  markModel.setList (bookmarks);
  emit startBook (startUrl);
  QString author = currentAuthors.join (tr(" & "));
  RecentBook book (currentTitle, author, 
                     QDateTime::currentDateTime().toTime_t(),
                     origBookFile);
  dbm.Write (book);
}

void
EpubDoc::ReadMeta (const QDomNodeList & metadata)
{
  currentAuthors.clear ();
  currentSubjects.clear ();
  QStringList titles;
  for (int i=0; i<metadata.count(); i++) {
    CollectTexts (metadata.at(i).toElement().elementsByTagName ("dc:creator"),
                  currentAuthors);
    CollectTexts (metadata.at(i).toElement().elementsByTagName ("dc:subject"),
                  currentSubjects);
    CollectTexts (metadata.at(i).toElement().elementsByTagName ("dc:title"),
                  titles);
  }
  qDebug () << __PRETTY_FUNCTION__ << " Authors " << currentAuthors;
  qDebug () << __PRETTY_FUNCTION__ << " Subjects " << currentSubjects;
  qDebug () << __PRETTY_FUNCTION__ << " Titles " << titles;
  currentTitle = titles.join (tr("; "));
}

void
EpubDoc::CollectTexts (const QDomNodeList & nodeList,
                       QStringList & values)
{
  for (int i=0; i<nodeList.count(); i++) {
    values.append (nodeList.at(i).toElement().text());
  }
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
