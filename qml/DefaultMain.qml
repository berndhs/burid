

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

  width: 600
  height: 400
  color: "yellow"
  Rectangle {
    id: titleBox
    width: parent.width * 0.75
    height: parent.height * 0.5
    anchors {
      horizontalCenter: parent.horizontalCenter
      top: parent.top
      topMargin: 50
    }
    color: "#f7f7f7"
    Text { 
      anchors.centerIn: parent
      style: Text.Sunken
      wrapMode: Text.Wrap
      text: mainRect.appTitle
    }
  }
  Component.onCompleted: {
    console.log ("done loaded main component!")
  }
}
