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
#include "image-provider.h"

int burid::PdfPager::msgcount(1000);

namespace burid
{
PdfPager::PdfPager (QObject *parent, int physDpiX, int physDpiY)
  :QObject(parent),
   imagePro (0),
   poppDoc (0),
   xresDefault (physDpiX),
   yresDefault (physDpiY)
{
  imagePro = new ImageProvider (QDeclarativeImageProvider::Image, this);
  xres = xresDefault;
  yres = yresDefault;
  qDebug () << __PRETTY_FUNCTION__ << " physX " << xres << " physY " << yres;
}

PdfPager::~PdfPager ()
{
}

int
PdfPager::pageNum ()
{
  return pagenum;
}

int
PdfPager::pageMax ()
{
  return pagemax;
}

void
PdfPager::setPageNum (int p)
{
  pagenum = p;
}

QDeclarativeImageProvider *
PdfPager::imageControl ()
{
  return imagePro;
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
PdfPager::PageImage (int pnum)
{
  qDebug () << __PRETTY_FUNCTION__  << pnum;
  if (poppDoc && (0 <= pnum) && (pnum < poppDoc->numPages())) {
    Poppler::Page * page = poppDoc->page (pnum);
    if (page) {
      return page->renderToImage (xres,yres);
    }
  }
  return QImage();
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
