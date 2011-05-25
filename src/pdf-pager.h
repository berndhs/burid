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

namespace burid
{

class PdfPager: public QObject, public QDeclarativeImageProvider
{
Q_OBJECT
public:
  PdfPager (QObject *parent=0);

  virtual QImage requestImage (const QString & id, 
                              QSize * size, 
                        const QSize & requestedSize);

  Q_INVOKABLE QString nextImage (const QString & direction, int offset);
  Q_INVOKABLE QString startImage ();

  void LoadPDF (const QString & filename);
  void LoadPDFfromData (const QByteArray & pdfData);

private:

  QImage PageImage (Poppler::Document * pdoc, int pnum);

  Poppler::Document    *poppDoc;
  int                   pagenum;
  int                   pagemax;

  static int pagecount;

} ;

} // namespace

#endif