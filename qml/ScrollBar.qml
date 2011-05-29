import QtQuick 1.0


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


Item {
  id: scrollBar
  property real position
  property real realtivePageSize
  property alias indicatorY: indicator.y
  property alias indicatorHeight: indicator.height
  property color backgroundColor: "green"
  property color indicatorColor: Qt.darker (backgroundColor, 2)

  Rectangle {
    id: background
    anchors.fill: parent
    color: backgroundColor
    opacity: 0.2
  }

  Rectangle {
    id: indicator
    x: 0
    y: scrollBar.position * scrollBar.height
    z: background.z + 1
    width: parent.width
    height: scrollBar.realtivePageSize * scrollBar.height
    color: indicatorColor
    opacity: 0.5
  }
}

