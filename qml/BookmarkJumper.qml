

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
  id: bookmarkJump
  property alias listModel: listView.model
  property int markCount:  -1
  property string forgetButtonColor: "#a4c470"
  property string forgetButtonFade: "#dbfbf7"
  property real  normalTextHeight: 32

  signal jumpTo (int row)
  signal forgetJump (int row)
  signal cancelJump ()

  width: 300
  height: 100
  color: "#77ff77"
  border.color: Qt.darker (color, 2.0)
  border.width: 2
  radius: 8

  Gradient {
    id: forgetButtonGradient
    GradientStop { position: 0.0; color: forgetButtonColor }
    GradientStop { position: 1.0; color: forgetButtonFade }
  }
  Rectangle {
    id: headlineRect
    width: parent.width
    height: 1.5 * normalTextHeight
    radius: bookmarkJump.radius
    color: "#ffff77"
    Text {
      anchors {
        verticalCenter: parent.verticalCenter;
        horizontalCenter: parent.horizontalCenter
      }
      text: qsTr ("Bookmarks")
    }
    MouseArea {
      anchors.fill: parent
      onClicked: { bookmarkJump.cancelJump () }
    }
  }

  Component {
    id: defaultDelegate
    Row {
      ChoiceButton {
        height: normalTextHeight
        labelText: qsTr ("forget")
        radius: 0.5 * height
        gradient: forgetButtonGradient
        onClicked: { bookmarkJump.forgetJump (index) }
      }
      Rectangle {
        height: normalTextHeight
        width: bookmarkJump.width * 0.7
        gradient: forgetButtonGradient
        radius: 0.5 * height
        Text {
          anchors { 
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: parent.radius 
          }
          text: qsTr ("jump to ") +  markTitle
        }
        MouseArea {
          anchors.fill: parent
          onClicked: { bookmarkJump.jumpTo (index) }
        }
      }
    }
  }

  ListView {
    id:  listView
    height: parent.height - headlineRect.height
    width: parent.width
    anchors {
      top: headlineRect.bottom;
      left: bookmarkJump.left
    }
    delegate: defaultDelegate
  }
}


