

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
  width: 600
  height: 600
  property string providerName: ""
  clip: true

  function loadImage (theName) {
    console.log ("load Image " + providerName + "  / " + theName)
    pageFlick.loadImage ("image://" + providerName + "/" + theName)
    console.log ("    load progress " + pageImage.progress)
  }
  focus: true
  
  Keys.onPressed: {
    console.log ("PDF key event " + event.key)
    pdfPagerIF.keyPressed (event.key, event.modifiers)
  }

  Connections {
    target: pdfPagerIF
    onUpdateImage: {
      console.log ("image update from IF " + updateName)
      bookPdfViewRect.loadImage (updateName)
    }
  }

  Flickable {
    id: pageFlick
    height: parent.height
    width: parent.width
    anchors { top: parent.top; left: parent.left }
    interactive: true
    contentWidth: pageImage.width
    contentHeight: pageImage.height
    contentX: 0
    contentY: 0

    function loadImage (theImage) {
      pageImage.source = theImage
      contentX = 0
      contentY = 0
      contentWidth = pageImage.paintedWidth
      contentHeight = pageImage.paintedHeight
      console.log (" loaded image " + theImage )
      console.log ("      cW " + contentWidth + "  cH " + contentHeight)
      console.log ("      progress " + pageImage.progress)
    }


    Image {
      id: pageImage
      z: parent.z + 2
      objectName: "PdfPageImage"
      source: "empty.png"
      asynchronous: false
      fillMode: Image.PreserveAspectCrop
    }
  }
  Rectangle {
    id: pageControlRect
    color: "lightgreen"
    opacity: 0.7
    height: 32
    width: parent.width
    z: parent.z+2
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
        onClicked: { bookPdfViewRect.loadImage (pdfPagerIF.nextImage("back",1)) }
      }
      ChoiceButton {
        id: forwardButton
        height: pageControlRect.height
        opacity: 0.7
        labelText: qsTr ("forward")
        onClicked: { bookPdfViewRect.loadImage (pdfPagerIF.nextImage("forward",1)) }
      }
    }
  }

}

