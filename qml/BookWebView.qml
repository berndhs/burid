import QtQuick 1.0
import QtWebKit 1.0


Rectangle {
  id: bookViewBox
  clip: true
  property alias currentPageY: bookViewFlick.contentY
  property string headLine: epubControlIF.contentPart

  function loadBook (theUrl)
  {
    bookView.loadPage (theUrl,"top")
  }
  function continueBook (theUrl, theOffset, theScale) 
  {
    bookView.loadPageMiddle (theUrl, theOffset, theScale)
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
        id: backPageButton
        height: pageControlRect.height
        opacity: 0.7
        labelText: qsTr ("<- Page")
        onClicked: { bookViewFlick.pageUp () }
      }
      ChoiceButton {
        id: backWebButton
        height: pageControlRect.height * 0.8
        opacity: 0.7
        labelText: qsTr ("back")
        onClicked: { bookView.back.trigger() }
      }
      ChoiceButton {
        id: gotoButton
        height: pageControlRect.height * 0.8
        opacity: 0.7
        labelText: qsTr ("To...")
        onClicked: { bookmarkJump.show () }
      }
      ChoiceButton {
        id: markButton
        height: pageControlRect.height * 0.8
        opacity: 0.7
        labelText: qsTr ("<b>!</b>")
        onClicked: {
          bookmarkInput.show ()
          bookmarkInput.setDefaultText (epubControlIF.nextBookmark())
        }
      }
      ChoiceButton {
        id: forwardWebButton
        height: pageControlRect.height * 0.8
        opacity: 0.7
        labelText: qsTr ("forward")
        onClicked: { bookView.forward.trigger() }
      }
      ChoiceButton {
        id: forwardPageButton
        height: pageControlRect.height
        opacity: 0.7
        labelText: qsTr ("Page ->")
        onClicked: { bookViewFlick.pageDown () }
      }
    }
  }

  BookmarkJumper {
    id: bookmarkJump
    width: bookViewBox.width * 0.9
    height: bookViewBox.height * 0.9
    anchors.centerIn: bookViewBox
    listModel: epubBookmarkModel
    markCount: -2
    color: "green"
    opacity: 0.0
    z: parent.z + 3
    function show ()
    {
      opacity = 0.85
      //setFocus (true)
    }
    function hide ()
    {
      opacity = 0.0
      //setFocus (false)
    }
  }

  StringEnter {
    id: bookmarkInput
    titleText: qsTr ("Bookmark Name")
    backgroundColor: "#77ddff"
    buttonTopColor: "#77ff77"
    inputValue: qsTr ("New Bookmark")
    width: parent.width * 0.8
    radius: 6
    z: parent.z + 3
    anchors.centerIn: bookViewBox
    opacity: 0.0
    function show ()
    {
      opacity = 0.9
      setFocus (true)
    }
    function hide ()
    {
      opacity = 0.0
      setFocus (false)
    }
    onDidEscape: {
      hide ()
    }
    onDidMark: {
      epubControlIF.mark(theMark, bookViewFlick.contentY, bookView.contentsScale)
      hide ()
    }
  }

  ScrollBar {
    id: bookViewScrollIndicator
    width: parent.height * 0.02
    height: bookViewBox.height
    anchors { right: parent.right; top: parent.top }
    opacity: 1
    z: bookViewFlick.z + 4
    position: (bookViewFlick.height > 0 ? bookViewFlick.visibleArea.yPosition : 1)
    realtivePageSize: (bookViewFlick.height > 0 ? bookViewFlick.visibleArea.heightRatio : 1)
  }

  Flickable {
    id: bookViewFlick
    height: parent.height
    width: parent.width
    contentWidth: bookView.width
    contentHeight: bookView.height
    function pageDown ()
    {
      if (bookViewFlick.atYEnd) {
        var nextUrl = epubControlIF.nextItem (1)
        if (nextUrl != "") {
          bookView.loadPage (nextUrl,"top")
        }
      } else {
        bookViewFlick.contentY += bookViewFlick.height * 0.9
      }
    }
    function pageUp ()
    {
      if (bookViewFlick.atYBeginning) {
        var nextUrl = epubControlIF.nextItem (-1)
        if (nextUrl != "") {
          bookView.loadPage (nextUrl, "bottom")
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
      property real continueY: 0
      property real continueScale: 1

      property string displayEnd: "top"

      function loadPage (theUrl, theEnd)
      {
        displayEnd = theEnd
        url = theUrl
      }
      function loadPageMiddle (theUrl, theOffset, theScale)
      {
        displayEnd = "middle"
        continueY = theOffset
        continueScale = theScale
        if (url == theUrl) {
          bookViewFlick.contentY = continueY
          contentsScale = continueScale
        } else {
          url = theUrl
        }
      }

      Keys.onLeftPressed: bookViewFlick.contentX += scrollXStep
      Keys.onRightPressed: bookViewFlick.contentX -= scrollXStep
      Keys.onUpPressed: bookViewFlick.contentY +=  scrollYStep
      Keys.onDownPressed: bookViewFlick.contentY -= scrollYStep
      Keys.onSpacePressed: { bookViewFlick.contentX = 0; bookViewFlick.contentY = 0 }
      Keys.onPressed: {
        if (event.key == Qt.Key_Plus)
        {
          bookView.contentsScale *= 1.5
        } else if (event.key == Qt.Key_Minus)
        {
          bookView.contentsScale /= 1.5
        } else if (event.key == Qt.Key_0)
        {
          bookView.contentsScale = origScale
        } else if (event.key == Qt.Key_PageUp)
        {
          bookViewFlick.pageUp ()
        } else if (event.key == Qt.Key_PageDown)
        {
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
        console.log ("load url finished " + displayEnd)
        isLoadFinished = true
        bookViewBox.color = "#d0e0ff"
        bookViewFlick.contentX = 0
        if (displayEnd == "top")
        {
          bookViewFlick.contentY = 0
        } else if (displayEnd == "bottom")
        {
          bookViewFlick.contentY = bookViewFlick.contentHeight - bookViewFlick.height
        } else if (displayEnd == "middle")
        {
          bookViewFlick.contentY = continueY
          contentsScale = continueScale
        }
      }
    }
  }
  Connections {
    target: bookmarkJump
    onJumpTo: {
      epubControlIF.jumpToBookmark (row)
      bookmarkJump.hide ()
    }
    onCancelJump: {
      bookmarkJump.hide ()
    }
    onForgetJump: {
      epubControlIF.removeMark (row)
    }
  }
  Component.onCompleted: {
    bookmarkJump.markCount = -3
  }
}