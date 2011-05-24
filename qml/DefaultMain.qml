

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


  signal quitApp ()
  signal startReading ()

  function loadHtml (theUrl) {
    bookContentView.bookUrl = theUrl
  }
  function readBook () {
    showReadBox ()
    startReading ()
  }
  function stopReading () {
    hideReadBox ()
  }
  function showReadBox () {
    bookViewRect.height = bookViewRect.visibleHeight
    bookViewRect.bookVisible = true
    titleBox.height = titleBox.hiddenHeight
    stopButton.visible = true
    bookButton.visible = false
    titleBox.showTitle = false
    mainRect.topReserve = 0
    loadHtml ("file:///home/bernd/myexperiments/burid/data/20/164/www.gutenberg.org@files@164@164-h@164-h-1.htm")
  }
  function hideReadBox () {
    bookViewRect.height = bookViewRect.hiddenHeight
    bookViewRect.bookVisible = false
    titleBox.height = titleBox.visibleHeight
    stopButton.visible = false
    bookButton.visible = true
    titleBox.showTitle = true
    mainRect.topReserve = 20
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
    height: 32
    width: parent.width
    property real buttonHeight: height
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

    Row {
      anchors.centerIn: parent
      spacing: parent.width * 0.1
      ChoiceButton {
        id: bookButton
        visible: true
        height: buttonRowRect.buttonHeight
        radius: height * 0.5
        labelText: qsTr ("Read Books")
        gradient: mainMenuButtonGradient
        onClicked: {
          mainRect.readBook ()
        }
      }
      ChoiceButton {
        id: stopButton
        visible: false
        height: buttonRowRect.buttonHeight
        radius: height * 0.5
        labelText: qsTr ("Stop Reading")
        gradient: mainMenuButtonGradient
        onClicked: {
          mainRect.stopReading ()
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
    id: bookViewRect
    width: parent.width
    property real visibleHeight: parent.height - buttonRowRect.height - parent.topReserve
    property real hiddenHeight: 0
    property bool bookVisible: false

    function ensureHeight () {
      visibleHeight = parent.height - buttonRowRect.height - parent.topReserve
      if (bookVisible) { height = visibleHeight }
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
    BookView {
      id: bookContentView
      width: parent.width
      height: parent.height
      anchors.horizontalCenter: parent.horizontalCenter
      color: "transparent"
      visible: bookViewRect.bookVisible
    }
  }

  onHeightChanged: {
    console.log ("Height changed to " + height)
    bookViewRect.ensureHeight()
  }
  Component.onCompleted: {
    console.log ("done loaded main component!")
  }
}
