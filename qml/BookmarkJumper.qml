
import QtQuick 1.0


Rectangle {
  id:
  bookmarkJump
  property alias listModel:
  listView.model
  property int markCount:  -1
  property string forgetButtonColor: "#a4c470"
  property string forgetButtonFade: "#dbfbf7"
  property real  normalTextHeight: 32

  signal jumpTo (int row)
  signal forgetJump (int row)
  signal cancelJump ()

  width: 300
  height: 100
  color: "green"
  border.color: Qt.darker (color, 2.0)
  border.width: 2
  radius: 8

  Gradient {
    id: forgetButtonGradient
    GradientStop { position: 0.0; color: forgetButtonColor }
    GradientStop { position: 1.0; color: forgetButtonFade }
  }
  Rectangle {
    id:
    headlineRect
    width: parent.width
    height: 1.5 * normalTextHeight
    radius: bookmarkJump.radius
    color: "yellow"
    Text {
      anchors {
        verticalCenter: parent.verticalCenter;
        horizontalCenter: parent.horizontalCenter
      }
      text: qsTr (" Bookmarks")
    }
    MouseArea {
      anchors.fill: parent
      onClicked: { bookmarkJump.cancelJump () }
    }
  }

  Component {
    id:
    defaultDelegate
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
    id:
    listView
    height:
    parent.height - headlineRect.height
    width:
    parent.width
    anchors {
      top:
      headlineRect.bottom;
      left:
      bookmarkJump.left
    }
    delegate: defaultDelegate
  }
  Component.onCompleted:
  {
    console.log ("BookmarkJumper completed w " + width + " h " + height)
    console.log ("       ListView          w " + listView.width + " h " + listView.height)
  }
}


