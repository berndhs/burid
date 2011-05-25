#include "pdf-pager.h"



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


#include <QDebug>

int burid::PdfPager::pagecount(1000);

namespace burid
{
PdfPager::PdfPager (QObject *parent)
  :QObject(parent),
   QDeclarativeImageProvider (QDeclarativeImageProvider::Image),
   poppDoc (0)
{
}

void 
PdfPager::LoadPDF (const QString & filename)
{
  poppDoc = Poppler::Document::load (filename);
  qDebug () << __PRETTY_FUNCTION__ << filename << poppDoc;
  pagenum = 0;
  if (poppDoc) {
    pagemax = poppDoc->numPages() - 1;
  } else {
    pagemax = 0;
  }
}

void 
PdfPager::LoadPDFfromData (const QByteArray & pdfData)
{
  poppDoc = Poppler::Document::loadFromData (pdfData);
  pagenum = 0;
  if (poppDoc) {
    pagemax = poppDoc->numPages() - 1;
  } else {
    pagemax = 0;
  }
}

QImage
PdfPager::PageImage (Poppler::Document * pdoc, int pnum)
{
  qDebug () << __PRETTY_FUNCTION__ << pdoc << pnum;
  if (pdoc && (0 <= pnum) && (pnum < pdoc->numPages())) {
    Poppler::Page * page = pdoc->page (pnum);
    if (page) {
      return page->renderToImage ();
    }
  }
  return QImage();
}

QImage
PdfPager::requestImage (const QString & id,
                            QSize * size,
                      const QSize & requestedSize)
{
  qDebug () << __PRETTY_FUNCTION__ << " want iamge " << id << requestedSize << size;
  QImage iconStart (QString("./images/icon128.png"),"PNG");
  QImage iconBack (QString("./images/icon64.png"));
  QImage iconForward (QString("./images/icon256.png"));
  QImage returnImage;
  if (id.startsWith (QString("start_"))) {
    pagenum = 0;
    returnImage = PageImage (poppDoc, pagenum);
  } else if (id.startsWith (QString ("back_"))) {
    if (pagenum > 0) { 
      pagenum--; 
    }
    returnImage = PageImage (poppDoc, pagenum);
  } else if (id.startsWith (QString ("forward_"))) {
    if (pagenum < pagemax) {
      pagenum++;
    }
    returnImage = PageImage (poppDoc, pagenum);
  }
  if (requestedSize.isValid()) {
    returnImage = returnImage.scaled (requestedSize.width(),requestedSize.height());
  }
  if (size) {
    *size = returnImage.size();
  }
  qDebug () << __PRETTY_FUNCTION__<< "    returning size " << returnImage.size();
  return returnImage;
}

QString
PdfPager::nextImage (const QString & direction, int offset)
{
  return direction + "_" + QString::number(pagecount++);
}

QString
PdfPager::startImage ()
{
  return QString("start_") + QString::number(pagecount++);
}

} // namespace
