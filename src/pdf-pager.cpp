#include "pdf-pager.h"
#include <QDebug>

int burid::PdfPager::pagecount(1000);

namespace burid
{
PdfPager::PdfPager (QObject *parent)
  :QObject(parent),
   QDeclarativeImageProvider (QDeclarativeImageProvider::Image)
{
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
    returnImage = iconStart;
  } else if (id.startsWith (QString ("back_"))) {
    returnImage = iconBack;
  } else if (id.startsWith (QString ("forward_"))) {
    returnImage = iconForward;
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
