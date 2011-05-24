#include "pdf-pager.h"
#include <QDebug>

namespace burid
{
PdfPager::PdfPager ()
  :QDeclarativeImageProvider (QDeclarativeImageProvider::Image)
{
}

QImage
PdfPager::requestImage (const QString & id,
                            QSize * size,
                      const QSize & requestedSize)
{
  qDebug () << __PRETTY_FUNCTION__ << " want iamge " << id;
  QImage icon (QString("./images/icon256.png"),"PNG");
  qDebug () << "    icon null " << icon.isNull();
  return icon;
}

} // namespace
