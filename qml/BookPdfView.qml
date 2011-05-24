

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
  id: bookPdfViewRect
  color: "red"
  property string providerName: ""
  Text { anchors.centerIn: parent; text: "PDF space" }
  clip: true

  function loadPdfFile (thePageFile) {
    pageImage.source = thePageFile
  }
  function loadImage (theName) {
    console.log ("load Image " + providerName + "  / " + theName)
    pageImage.source = "image://" + providerName + "/" + theName
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
        onClicked: { bookPdfViewRect.loadImage ("back") }
      }
      ChoiceButton {
        id: forwardButton
        height: pageControlRect.height
        opacity: 0.7
        labelText: qsTr ("forward")
        onClicked: { bookPdfViewRect.loadImage ("forward") }
      }
    }
  }

  Flickable {
    id: pageFlick
    Image {
      id: pageImage
      objectName: "PdfPageImage"
      height: parent.height
      width: parent.width
      fillMode: Image.PreserveAspectCrop
    }
  }
}

