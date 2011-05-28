#include "image-provider.h"



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
#include <QSize>

namespace burid
{

ImageProvider::ImageProvider (QDeclarativeImageProvider::ImageType imt,
                 PdfPager * pager)
  :QDeclarativeImageProvider (imt),
   mPager (pager)
{
}

QImage
ImageProvider::requestImage (const QString & id,
                            QSize * size,
                      const QSize & requestedSize)
{
  qDebug () << __PRETTY_FUNCTION__ << " want iamge " << id << requestedSize << size;
  qDebug () << " pager " << mPager;
  QImage returnImage;
  if (mPager == 0) {
    return returnImage;
  }
  int pagenum = mPager->pageNum ();
  int pagemax = mPager->pageMax ();
  if (id.startsWith (QString("start_"))) {
    pagenum = 0;
    returnImage = mPager->PageImage (pagenum);
  } else if (id.startsWith (QString ("back_"))) {
    if (pagenum > 0) { 
      pagenum--; 
    }
    returnImage = mPager->PageImage (pagenum);
  } else if (id.startsWith (QString ("forward_"))) {
    if (pagenum < pagemax) {
      pagenum++;
    }
    returnImage = mPager->PageImage (pagenum);
  } else if (id.startsWith (QString("update_"))) {
    returnImage = mPager->PageImage (pagenum);
  }
  qDebug () << "              incoming size " << *size;
  if (size) {
    *size = returnImage.size();
  }
  mPager->setPageNum (pagenum);
  qDebug () << __PRETTY_FUNCTION__<< "    returning size " << returnImage.size();
  return returnImage;
}

QPixmap 
ImageProvider::requestPixmap (const QString & id, 
                              QSize * size, 
                        const QSize & requestedSize)
{
  qDebug () << __PRETTY_FUNCTION__ << id << size << requestedSize;
  return QPixmap::fromImage (requestImage (id,size,requestedSize));
}


} // namespace