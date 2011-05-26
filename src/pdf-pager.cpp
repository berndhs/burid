#include "pdf-pager.h"

#include <Qt>

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
#include <QLabel>
#include <QPixmap>

int burid::PdfPager::msgcount(1000);

namespace burid
{
PdfPager::PdfPager (QObject *parent)
  :QObject(parent),
   QDeclarativeImageProvider (QDeclarativeImageProvider::Image),
   poppDoc (0),
   xresDefault (72.0),
   yresDefault (72.0)
{
  xres = xresDefault;
  yres = yresDefault;
}

PdfPager::~PdfPager ()
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

QStringList
PdfPager::infoKeys ()
{
  if (poppDoc) {
    return poppDoc->infoKeys();
  } else {
    QStringList list;
    list << "No Document";
    return list;
  }
}

QString
PdfPager::info (const QString & key)
{
  if (poppDoc) {
    return poppDoc->info(key);
  }
  return QString();
}

void
PdfPager::keyPressed (int key, int modifier)
{
  qDebug () << __PRETTY_FUNCTION__ << " key " <<  key << " mod " << modifier;
  QString updateArg (QString("update_") + QString::number(++msgcount)) ;
  switch (key) {
  case Qt::Key_Plus:
    qDebug () << "    Plus pressed";
    xres *= 1.1; yres *= 1.1;
    emit updateImage (updateArg);
    break;
  case Qt::Key_Minus:
    xres /= 1.1; yres /= 1.1;
    emit updateImage (updateArg);
    break;
  case Qt::Key_0:
    xres = xresDefault; yres = yresDefault;
    emit updateImage (updateArg);
  default:
    break;
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
      return page->renderToImage (xres,yres);
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
  } else if (id.startsWith (QString("update_"))) {
    returnImage = PageImage (poppDoc, pagenum);
  }
  qDebug () << "              incoming size " << *size;
  if (size) {
    *size = returnImage.size();
  }
  qDebug () << __PRETTY_FUNCTION__<< "    returning size " << returnImage.size();
  return returnImage;
}

QString
PdfPager::nextImage (const QString & direction, int offset)
{
  return direction + "_" + QString::number(++msgcount);
}

QString
PdfPager::startImage ()
{
  return QString("start_") + QString::number(++msgcount);
}

} // namespace
