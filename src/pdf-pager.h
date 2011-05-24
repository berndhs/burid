#ifndef BURID_PDF_PAGER_H
#define BURID_PDF_PAGER_H

#include <QDeclarativeImageProvider>
#include <QImage>

namespace burid
{

class PdfPager: public QDeclarativeImageProvider
{
public:
  PdfPager ();

  virtual QImage requestImage (const QString & id, 
                              QSize * size, 
                        const QSize & requestedSize);

} ;

} // namespace

#endif