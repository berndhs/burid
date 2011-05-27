
import QtQuick 1.0

Rectangle {
  id: inputPrompt

  property real titleHeight: 32
  property real inputHeight: 32
  property real heightSpacing: 4
  property real buttonWidth: width * 0.2
  property alias titleText: titleBarText.text
  property alias inputValue: inputBarInput.text
  property color backgroundColor: "#ffaa22"
  property color titleColor: Qt.lighter (backgroundColor)
  property color inputColor: "#f7f7f7"
  property color buttonTopColor: titleColor
  property color buttonBottomColor: Qt.lighter (buttonTopColor, 2)

  signal didMark (string theMark)
  signal didEscape ()

  function setFocus (hasFocus) {
    inputBarInput.focus = hasFocus
  }
  function setDefaultText (theText) {
    inputValue = theText
  }

  color: backgroundColor
  width: 500
  height: titleHeight + inputHeight + 2*heightSpacing

  Column {
    spacing: inputPrompt.heightSpacing
    Rectangle {
      id: titleBar
      color: inputPrompt.titleColor
      anchors.horizontalCenter: parent.hoziontalCenter
      width: inputPrompt.width
      height: inputPrompt.titleHeight
      radius: inputPrompt.radius
      Text {
        id: titleBarText
        anchors.centerIn: parent
        text: qsTr ("Title")
      }
      Gradient {
        id: buttonGradient
        GradientStop { position: 0; color: inputPrompt.buttonTopColor }
        GradientStop { position: 1.0; color: inputPrompt.buttonBottomColor }
      }
      ChoiceButton {
        height: inputPrompt.titleHeight
        width: inputPrompt.buttonWidth
        commonMargin: 0
        radius: height * 0.5
        gradient: buttonGradient
        anchors { left: parent.left; top: parent.top }
        labelText: qsTr ("Cancel")
        onClicked: { inputPrompt.didEscape () }
      }
      ChoiceButton {
        height: inputPrompt.titleHeight
        width: inputPrompt.buttonWidth
        commonMargin: 0
        radius: height * 0.5
        gradient: buttonGradient
        anchors { right: parent.right; top: parent.top }
        labelText: qsTr ("Mark")
        onClicked: { inputPrompt.didMark (inputBarInput.text) }
      }
    }
    Rectangle {
      id: inputBar
      color: inputPrompt.inputColor
      width: inputPrompt.width
      height: inputPrompt.inputHeight
      radius: inputPrompt.radius
      TextInput {
        id: inputBarInput
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        text: qsTr ("Input")
        Keys.onEscapePressed: { inputPrompt.didEscape() }
        Keys.onReturnPressed: { inputPrompt.didMark (text) }
        Keys.onEnterPressed:  { inputPrompt.didMark (text) }
      }
    }
  }
}