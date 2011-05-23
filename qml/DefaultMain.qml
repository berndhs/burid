

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

  property string appTitle: "BRead Book Reader"
  property real topReserve: 20

  signal quitApp ()
  signal startReading ()

  function readBook () {
    showReadBox ()
    startReading ()
  }
  function stopReading () {
    hideReadBox ()
  }
  function showReadBox () {
    bookViewRect.height = bookViewRect.visibleHeight
    titleBox.height = titleBox.hiddenHeight
  }
  function hideReadBox () {
    bookViewRect.height = bookViewRect.hiddenHeight
    titleBox.height = titleBox.visibleHeight
  }

  width: 600
  height: 400
  color: "yellow"

  Rectangle {
    id: titleBox
    width: parent.width * 0.75
    property real visibleHeight: parent.height * 0.25
    property real hiddenHeight: 0
    height: visibleHeight
    anchors {
      horizontalCenter: parent.horizontalCenter
      top: parent.top
      topMargin: topReserve
    }
    color: "#f7f7f7"
    Text { 
      anchors.centerIn: parent
      style: Text.Sunken
      wrapMode: Text.Wrap
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

    Row {
      anchors.centerIn: parent
      spacing: 4
      ChoiceButton {
        id: bookButton
        height: buttonRowRect.buttonHeight
        labelText: qsTr ("Read Books")
        onClicked: {
          mainRect.readBook ()
        }
      }
      ChoiceButton {
        id: stopButton
        height: buttonRowRect.buttonHeight
        labelText: qsTr ("Stop Reading")
        onClicked: {
          mainRect.stopReading ()
        }
      }
      ChoiceButton {
        id: quitButton
        height: buttonRowRect.buttonHeight
        labelText: qsTr ("Quit")
        onClicked: {
          mainRect.quitApp ()
        }
      }
    }

  }

  Rectangle {
    id: bookViewRect
    width: parent.width
    property real visibleHeight: parent.height - buttonRowRect.height - titleBox.height - parent.topReserve
    property real hiddenHeight: 0
    height: hiddenHeight
    color: "lightblue"
    anchors {
      top: buttonRowRect.bottom
      left: parent.left
    }
    Behavior  on height {
      NumberAnimation { duration: 250 }
    }
  }
  Component.onCompleted: {
    console.log ("done loaded main component!")
  }
}
