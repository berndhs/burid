import QtQuick 1.0
import QtWebKit 1.0


Rectangle {
  id: bookViewBox
  clip: true

  function loadBook (theUrl) {
    bookView.loadPage (theUrl)
  }

  Rectangle {
    id: pageControlRect
    color: "transparent"
    height: 32
    width: parent.width
    z: parent.z+1
    anchors {
      horizontalCenter: parent.horizontalCenter
      bottom: parent.bottom
    }
    Row {
      spacing: 16
      anchors.centerIn: parent
      ChoiceButton {
        id: backButton
        height: pageControlRect.height
        opacity: 0.7
        labelText: qsTr ("back")
        onClicked: { bookViewFlick.pageUp () }
      }
      ChoiceButton {
        id: forwardButton
        height: pageControlRect.height
        opacity: 0.7
        labelText: qsTr ("forward")
        onClicked: { bookViewFlick.pageDown () }
      }
    }
  }
  Flickable {
 
    id: bookViewFlick
    height: parent.height
    width: parent.width
    contentWidth: bookView.width
    contentHeight: bookView.height
    function pageDown () {
      if (bookViewFlick.atYEnd) {
        var nextUrl = epubControlIF.nextItem (1)
        if (nextUrl != "") {
          bookView.loadPage (nextUrl)
        }
      } else {
        bookViewFlick.contentY += bookViewFlick.height * 0.9 
      }
    }
    function pageUp () {
      if (bookViewFlick.atYBeginning) {
        var nextUrl = epubControlIF.nextItem (-1)
        if (nextUrl != "") {
          bookView.loadPage (nextUrl)
        }
      } else {
        bookViewFlick.contentY -= bookViewFlick.height * 0.9 
      }
    }

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

      function loadPage (theUrl) {
        url = theUrl
        bookViewFlick.contentX = 0
        bookViewFlick.contentY = 0
      }    

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
           bookViewFlick.pageUp ()
         } else if (event.key == Qt.Key_PageDown) {
           bookViewFlick.pageDown ()
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