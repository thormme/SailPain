#include <zenilib.h>
#include "Input.h"

bool Input::keys[] = {0};
bool Input::keysPressed[] = {0};
bool Input::keysReleased[] = {0};

Sint16 Input::axisValue[128][128] = {0};
bool Input::joyButtons[128][1024] = {0};
bool Input::joyButtonsPressed[128][1024] = {0};
bool Input::joyButtonsReleased[128][1024] = {0};

void Input::updateKey(SDLKey key, bool isDown) {
	keys[(int)key] = isDown;
	if (isDown) {
		keysPressed[(int)key] = true;
	} else {
		keysReleased[(int)key] = true;
	}
}

bool Input::isKeyDown(SDLKey key) {
	return keys[(int)key];
}

bool Input::isKeyPressed(SDLKey key) {
	return keysPressed[(int)key];
}

bool Input::isKeyReleased(SDLKey key) {
	return keysReleased[(int)key];
}

void Input::updateJoyButton(Uint8 joystickIndex, Uint8 button, bool isDown) {
	joyButtons[(int)joystickIndex][(int)button] = isDown;
	if (isDown) {
		joyButtonsPressed[(int)joystickIndex][(int)button] = true;
	} else {
		joyButtonsReleased[(int)joystickIndex][(int)button] = true;
	}
}

bool Input::isJoyButtonDown(Uint8 joystickIndex, Uint8 button) {
	return joyButtons[(int)joystickIndex][(int)button];
}

bool Input::isJoyButtonPressed(Uint8 joystickIndex, Uint8 button) {
	return joyButtonsPressed[(int)joystickIndex][(int)button];
}

bool Input::isJoyButtonReleased(Uint8 joystickIndex, Uint8 button) {
	return joyButtonsReleased[(int)joystickIndex][(int)button];
}

void Input::updateJoyAxisValue(Uint8 joystickIndex, Uint8 axisIndex, Sint16 value) {
	axisValue[(int)joystickIndex][(int)axisIndex] = value;
}

double Input::getJoyAxisValue(Uint8 joystickIndex, Uint8 axisIndex) {
	return (double)(axisValue[(int)joystickIndex][(int)axisIndex])/32768.0;
}

void Input::stepInput() {
	memset(keysPressed, 0, 1024 * sizeof keysPressed[0]);
	memset(keysReleased, 0, 1024 * sizeof keysReleased[0]);
	memset(joyButtonsPressed, 0, 128 * 1024 * sizeof joyButtonsPressed[0][0]);
	memset(joyButtonsReleased, 0, 128 * 1024 * sizeof joyButtonsReleased[0][0]);
}


InputAction::InputAction() {

}

InputAction::InputAction(SDLKey key, ButtonState buttonState) {
	m_inputType = KEY;
	m_key = key;
	m_buttonState = buttonState;
}

InputAction::InputAction(Uint8 joyIndex, Uint8 joyButton, ButtonState buttonState) {
	m_inputType = BUTTON;
	m_joyIndex = joyIndex;
	m_joyButton = joyButton;
	m_buttonState = buttonState;
}

InputAction::InputAction(Uint8 joyIndex, Uint8 joyAxis, double deadZoneMin, double deadZoneMax) {
	m_inputType = AXIS;
	m_joyIndex = joyIndex;
	m_joyAxis = joyAxis;
	m_deadZoneMin = deadZoneMin;
	m_deadZoneMax = deadZoneMax;
}

const bool InputAction::isActive() const {
	double joyValue;
	switch (m_inputType) {
	case KEY:
		switch (m_buttonState) {
		case PRESSED:
			return Input::isKeyPressed(m_key);
		case RELEASED:
			return Input::isKeyReleased(m_key);
		case DOWN:
		default:
			return Input::isKeyDown(m_key);
		}
	case BUTTON:
		switch (m_buttonState) {
		case PRESSED:
			return Input::isJoyButtonPressed(m_joyIndex, m_joyButton);
		case RELEASED:
			return Input::isJoyButtonReleased(m_joyIndex, m_joyButton);
		case DOWN:
		default:
			return Input::isJoyButtonDown(m_joyIndex, m_joyButton);
		}
	case AXIS:
		joyValue = Input::getJoyAxisValue(m_joyIndex, m_joyAxis);
		return joyValue > m_deadZoneMax || joyValue < m_deadZoneMin;
	default:
		return false;
	}
}

const double InputAction::getDegree() const {
	switch (m_inputType) {
	case KEY:
	case BUTTON:
		return (double)isActive();
	case AXIS:
		if (isActive()) {
			return Input::getJoyAxisValue(m_joyIndex, m_joyAxis);
		} else {
			return 0.0;
		}
	default:
		return 0.0;
	}
}