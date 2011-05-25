#ifndef BURID_PDF_PAGER_H
#define BURID_PDF_PAGER_H

#include <QDeclarativeImageProvider>
#include <QImage>
#include <QObject>

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

private:

  static int pagecount;

} ;

} // namespace

#endif