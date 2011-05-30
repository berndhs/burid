

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

import QtQuick 1.0

Rectangle {
  id: mainRect
  objectName: "DefaultMain_mainRect"

  property string appTitle: "BuRid Book Reader"
  property real topReserve: 20
  property string mainMenuButtonColor: "#b4a470"
  property string mainMenuButtonFade: "#fbdff7"

  property alias epubPageY: bookWebContentView.currentPageY

  signal quitApp ()
  signal startReadEpub ()
  signal startReadPdf ()
  signal finishedBook ()

  function loadEpub (theUrl)
  {
    showReadBox ("html")
    bookWebContentView.loadBook (theUrl)
  }
  function continueEpub (theUrl, theOffset, theScale)
  {
    showReadBox ("html")
    bookWebContentView.continueBook (theUrl, theOffset, theScale)
  }
  function loadPdf (thePage)
  {
    bookPdfContentView.loadImage (thePage)
  }
  function readBook ()
  {
    startReadEpub ()
    console.log (" DefaultMain readBook")
  }
  function readPdf ()
  {
    startReadPdf ()
    console.log (" DefaultMain readPdf")
    showReadBox ("pdf")
  }
  function stopReading ()
  {
    hideReadBox ()
  }
  function recentEpubs ()
  {
    console.log ("Recent Epubs")
    recentEpubRect.show ()
    titleBox.height = titleBox.hiddenHeigh
    showMainSelection (false)
    mainRect.topReserve = 0
  }
  function showReadBox (theFormat)
  {
    if (theFormat == "html") {
      bookWebViewRect.show ()
      bookPdfViewRect.hide ()
    } else if (theFormat == "pdf") {
      bookWebViewRect.hide ()
      bookPdfViewRect.show ()
    }
    recentEpubRect.hide ()
    titleBox.height = titleBox.hiddenHeight
    showMainSelection (false)
    mainRect.topReserve = 0
    if (theFormat == "pdf") {
      loadPdf (pdfPagerIF.startImage())
    }
  }
  function hideReadBox ()
  {
    bookWebViewRect.hide ()
    bookPdfViewRect.hide ()
    recentEpubRect.hide ()
    titleBox.height = titleBox.visibleHeight
    showMainSelection (true)
    mainRect.topReserve = 20
  }
  function showMainSelection (visi) {
    bookEpubButton.visible = visi
    bookPdfButton.visible = visi
    recentEpubButton.visible = visi
    titleBox.showTitle = visi
    stopButton.visible = !visi
  }

  width: 600
  height: 400
  color: "yellow"

  Rectangle {
    id: titleBox
    width: parent.width * 0.75
    property real visibleHeight: parent.height * 0.25
    property real hiddenHeight: 0
    property bool showTitle: true
    height: visibleHeight
    radius: 16
    anchors {
      horizontalCenter: parent.horizontalCenter
      top: parent.top
      topMargin: topReserve
    }
    color: "#f7f7f7"
    Text {
      anchors.centerIn: parent
      visible: titleBox.showTitle
      style: Text.Sunken
      wrapMode: Text.Wrap
      horizontalAlignment: Text.AlignHCenter
      text: mainRect.appTitle
    }
    Behavior  on height {
      NumberAnimation { duration: 250 }
    }
  }

  Rectangle {
    id: buttonRowRect
    height: childrenRect.height
    width: parent.width
    property real buttonHeight: 32
    color: "transparent"
    anchors {
      top: titleBox.bottom
      topMargin: 4
      horizontalCenter: mainRect.horizontalCenter
    }
    Gradient {
      id: mainMenuButtonGradient
      GradientStop { position: 0.0; color: mainMenuButtonColor }
      GradientStop { position: 1.0; color: mainMenuButtonFade }
    }

    Rectangle {
      id: epubContentHead
      color: "transparent"
      width: childrenRect.width
      height: parent.height
      visible: bookWebViewRect.bookVisible
      anchors { left: parent.left; verticalCenter: parent.verticalCenter }
      Text { text: bookWebContentView.headLine }
    }

    Row {
      anchors.centerIn: parent
      spacing: parent.width * 0.1
      Row {
        spacing: 8
        ChoiceButton {
          id: bookEpubButton
          visible: true
          height: buttonRowRect.buttonHeight
          radius: height * 0.5
          labelText: qsTr ("Open EPub")
          gradient: mainMenuButtonGradient
          onClicked: {
            mainRect.readBook ()
          }
        }
        ChoiceButton {
          id: recentEpubButton
          visible: true
          height: buttonRowRect.buttonHeight
          radius: height * 0.5
          labelText: qsTr ("Recent EPub")
          gradient: mainMenuButtonGradient
          onClicked: {
            mainRect.recentEpubs ()
          }
        }
      }
      ChoiceButton {
        id: bookPdfButton
        visible: true
        height: buttonRowRect.buttonHeight
        radius: height * 0.5
        labelText: qsTr ("Read Pdf")
        gradient: mainMenuButtonGradient
        onClicked: {
          mainRect.readPdf ()
        }
      }
      ChoiceButton {
        id: stopButton
        visible: false
        height: buttonRowRect.buttonHeight
        radius: height * 0.5
        labelText: qsTr ("Close")
        gradient: mainMenuButtonGradient
        onClicked: {
          mainRect.stopReading ()
          mainRect.finishedBook ()
        }
      }
      ChoiceButton {
        id: quitButton
        height: buttonRowRect.buttonHeight
        radius: height * 0.5
        labelText: qsTr ("Quit")
        gradient: mainMenuButtonGradient
        onClicked: {
          mainRect.quitApp ()
        }
      }
    }
  }

  Rectangle {
    id: bookWebViewRect
    width: parent.width
    property real visibleHeight: parent.height - buttonRowRect.height - parent.topReserve
    property real hiddenHeight: 0
    property bool bookVisible: false

    function show ()
    {
      height = visibleHeight
      bookVisible = true
    }
    function hide ()
    {
      height = 0
      bookVisible = false
    }
    function ensureHeight ()
    {
      visibleHeight = parent.height - buttonRowRect.height - parent.topReserve
      if (bookVisible) {
        height = visibleHeight
      }
    }

    height: hiddenHeight
    color: "#f0f0ff"
    anchors {
      top: buttonRowRect.bottom
      left: parent.left
    }
    Behavior  on height {
      NumberAnimation { duration: 250 }
    }
    BookWebView {
      id: bookWebContentView
      width: parent.width
      height: parent.height
      anchors.horizontalCenter: parent.horizontalCenter
      color: "transparent"
      visible: bookWebViewRect.bookVisible
    }
  }

  Rectangle {
    id: recentEpubRect
    property real visibleHeight:  parent.height - buttonRowRect.height - parent.topReserve
    property real hiddenHeight: 0
    property bool listVisible: false

    width: parent.width
    height: hiddenHeight
    anchors {
      top: buttonRowRect.bottom
      left: parent.left
    }

    function show ()
    {
      height = visibleHeight
      listVisible = true
    }
    function hide ()
    {
      height = 0
      listVisible = false
    }

    Behavior  on height {
      NumberAnimation { duration: 250 }
    }
    RecentEpub {
      width: parent.width
      height: parent.height
      listModel: recentEpubModel
      visible: recentEpubRect.listVisible
      onOpenBook: {
        console.log ("open recent book " + row)
        epubControlIF.openRecentBook (row)
      }
      onForgetBook: {
        console.log ("forget recent book " + row)
        epubControlIF.forgetRecentBook (row)
      }
      onCancelSelect: {
        console.log ("cancel view recent books")
        mainRect: hideReadBox ()
      }
    }
  }

  Rectangle {
    id: bookPdfViewRect
    width: parent.width
    property real visibleHeight: parent.height - buttonRowRect.height - parent.topReserve
    property real hiddenHeight: 0
    property bool bookVisible: false

    function show ()
    {
      height = visibleHeight
      bookVisible = true
    }
    function hide ()
    {
      height = 0
      bookVisible = false
    }
    function ensureHeight ()
    {
      visibleHeight = parent.height - buttonRowRect.height - parent.topReserve
      if (bookVisible) {
        height = visibleHeight
      }
    }

    height: hiddenHeight
    color: "#f7f0f0"
    anchors {
      top: buttonRowRect.bottom
      left: parent.left
    }
    Behavior  on height {
      NumberAnimation { duration: 250 }
    }
    BookPdfView {
      id: bookPdfContentView
      width: parent.width
      height: parent.height
      anchors.horizontalCenter: parent.horizontalCenter
      color: "transparent"
      visible: bookPdfViewRect.bookVisible
      providerName: "pdfpager"
    }
  }

  onHeightChanged: {
    console.log ("Height changed to " + height)
    bookWebViewRect.ensureHeight()
    bookPdfViewRect.ensureHeight()
  }
  Component.onCompleted: {
    console.log ("done loaded main component!")
  }
}
