
import QtQuick 1.0


Rectangle {
  id:
  bookmarkJump
  property alias listModel:
  listView.model
  property int markCount:  -1

  signal jumpTo (int row)
  signal cancelJump ()

  width: 300
  height: 100
  color: "green"
  Rectangle {
    id:
    headlineRect
    width:
    parent.width
    height:
    childrenRect.height
    color: "yellow"
    Text {
      anchors {
        top:
        parent.top;
        horizontalCenter:
        parent.horizontalCenter
      }
      text:
      markCount + qsTr (" Bookmarks")
    }
    MouseArea {
      anchors.fill: parent
      onClicked: { bookmarkJump.cancelJump () }
    }
  }

  Component {
    id:
    defaultDelegate
    Rectangle {
      height: 32
      width:
      bookmarkJump.width * 0.5
      color: "lightblue"
      Text {
        text: qsTr ("jump to ") +  markTitle
      }
      MouseArea {
        anchors.fill: parent
        onClicked: { bookmarkJump.jumpTo (index) }
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
    delegate:
    defaultDelegate
  }
  Component.onCompleted:
  {
    console.log ("BookmarkJumper completed w " + width + " h " + height)
    console.log ("       ListView          w " + listView.width + " h " + listView.height)
  }
}


