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
#include <QDebug>

namespace burid
{
EpubDoc::EpubDoc (QObject *parent)
  :QObject (parent)
{
}

QString
EpubDoc::nextPage (const QString & direction, int offset)
{
  qDebug () << __PRETTY_FUNCTION__ << direction << offset;
  return direction + "_" + QString::number(offset);
}

QString
EpubDoc::startPage ()
{
  qDebug () << __PRETTY_FUNCTION__;
  return QString ("start_");
}

void
EpubDoc::openBook (const QString & filename)
{
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
    qDebug () << __PRETTY_FUNCTION__ << doc.toString(1);
  }
  qDebug () << __PRETTY_FUNCTION__ << " file " << tmpname << ok;
}

void
EpubDoc::unzip (const QString & compressedName, QString & clearName)
{
  QString tmpdir = QDesktopServices::storageLocation (QDesktopServices::TempLocation);
  tmpdir.append (QDir::separator());
  tmpdir.append ("burid-unzip");
  QProcess::execute (QString ("unzip ")
                     + compressedName
                     + QString (" -d ")
                     + tmpdir);
  clearName = tmpdir + QDir::separator() + QString("content.opf"); 
  qDebug () << __PRETTY_FUNCTION__ << "  tmp unzipped should be " << clearName;
}

void
EpubDoc::clearCache ()
{
}

} // namespace