import QtQuick 1.0
import QtWebKit 1.0


Rectangle {
  id: bookViewBox
  property alias bookUrl: bookView.url
  clip: true

  Flickable {
 
    id: bookViewFlick
    height: parent.height
    width: parent.width

    WebView {
      id: bookView
      objectName: "BookWebView"
      settings.autoLoadImages: true
      settings.pluginsEnabled: true
      settings.privateBrowsingEnabled: true
      html: "<p>default <b>html</b>.</p><p>Paragraphs</p>"
      property bool isLoadFinished : false
      property real origScale: 1
      property real scrollXStep: -5
      property real scrollYStep: -5
      Keys.onLeftPressed: bookViewFlick.contentX += scrollXStep 
      Keys.onRightPressed: bookViewFlick.contentX -= scrollXStep 
      Keys.onUpPressed: bookViewFlick.contentY +=  scrollYStep
      Keys.onDownPressed: bookViewFlick.contentY -= scrollYStep
      Keys.onSpacePressed: { bookViewFlick.contentX = 0; bookViewFlick.contentY = 0 }
      Keys.onPressed: {
         if (event.key == Qt.Key_Plus) { bookView.contentsScale += 0.1 } 
         else if (event.key == Qt.Key_Minus) {bookView.contentsScale -= 0.1 }
         else if (event.key == Qt.Key_0) {bookView.contentsScale = origScale }
         else if (event.key == Qt.Key_PageUp) {
           bookViewFlick.contentY -= bookViewFlick.height * 0.9 
         } else if (event.key == Qt.Key_PageDown) {
           bookViewFlick.contentY += bookViewFlick.height * 0.9 
         }
      }
      preferredWidth: bookViewBox.width
      preferredHeight: bookViewBox.height
      contentsScale: 1

      onUrlChanged: {
        console.log ("url changed to " + url)
        isLoadFinished = false
        bookViewBox.color = "red"
        bookViewFlick.contentX = 0
        bookViewFlick.contentY = 0
      }
      onLoadFinished: {
        console.log ("load url finished ")
        isLoadFinished = true
        bookViewBox.color = "blue"
      }
    }
  }
}