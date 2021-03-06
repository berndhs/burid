#ifndef BURID_PDF_PAGER_H
#define BURID_PDF_PAGER_H


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


#include <QDeclarativeImageProvider>
#include <QImage>
#include <QObject>
#include <poppler/qt4/poppler-qt4.h>
#include <QStringList>
#include <QKeyEvent>

namespace burid
{

class ImageProvider;

class PdfPager: public QObject
{
Q_OBJECT
public:
  PdfPager (QObject *parent=0, int physDpiX=72, int physDpiY=72);
  ~PdfPager ();


  Q_INVOKABLE QString nextImage (const QString & direction, int offset);
  Q_INVOKABLE QString startImage ();
  Q_INVOKABLE void keyPressed (int key, int modfier);
  Q_INVOKABLE QDeclarativeImageProvider * imageControl ();

  void LoadPDF (const QString & filename);
  void LoadPDFfromData (const QByteArray & pdfData);

  QStringList infoKeys ();
  QString     info (const QString & key);

  int pageNum ();
  void setPageNum (int p);

  int pageMax ();

  QImage PageImage (int pnum);

signals:

  void updateImage (QString updateName);

private:

  ImageProvider        *imagePro;

  Poppler::Document    *poppDoc;
  int                   pagenum;
  int                   pagemax;
  double                xres;
  double                yres;
  double                xresDefault;
  double                yresDefault;

  static int msgcount;

} ;

} // namespace

#endif