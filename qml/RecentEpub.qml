
import QtQuick 1.0

Rectangle {
  id: recentEpub
  property alias listModel: listView.model
  property string forgetButtonColor: "#a4c470"
  property string forgetButtonFade: "#dbfbf7"
  property real  normalTextHeight: 32

  signal openBook (int row)
  signal forgetBook (int row)
  signal cancelSelect ()

  color: "#efefbf"
  border.color: Qt.darker (color, 2.0)
  border.width: 3
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
    radius: recentEpub.radius
    color: Qt.lighter (recentEpub.color)
    Text {
      anchors {
        verticalCenter: parent.verticalCenter;
        horizontalCenter: parent.horizontalCenter
      }
      text: qsTr ("Recent Epub Books")
    }
    MouseArea {
      anchors.fill: parent
      onClicked: { recentEpub.cancelSelect () }
    }
  }
  Component {
    id: defaultDelegate
    Row {
      spacing: 2
      ChoiceButton {
        height: normalTextHeight
        labelText: qsTr ("forget")
        radius: 0.5 * height
        gradient: forgetButtonGradient
        onClicked: { recentEpub.forgetBook (index) }
      }
      Rectangle {
        height: normalTextHeight
        width: recentEpub.width * 0.7
        gradient: forgetButtonGradient
        radius: 0.5 * height
        Text {
          anchors { 
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: parent.radius 
          }
          wrapMode: Text.Wrap
          text: author + ": " + title
        }
        MouseArea {
          anchors.fill: parent
          onClicked: { recentEpub.openBook (index) }
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
      left: recentEpub.left
    }
    delegate: defaultDelegate
  }
}