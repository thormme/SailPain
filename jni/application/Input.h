#ifndef INPUT_H
#define INPUT_H
#include <zenilib.h>

class InputAction {

public:
	enum ButtonState { PRESSED, RELEASED, DOWN};

	InputAction();
	InputAction(SDLKey key, ButtonState buttonState = DOWN);
	InputAction(Uint8 joyIndex, Uint8 joyButton, ButtonState buttonState = DOWN);
	InputAction(Uint8 joyIndex, Uint8 joyAxis, double deadZoneMin = 0.0, double deadZoneMax = 0.0);

	const bool isActive() const;
	const double getDegree() const;

private:
	enum InputType { KEY, BUTTON, AXIS};

	InputType m_inputType;

	double m_deadZoneMin;
	double m_deadZoneMax;
	ButtonState m_buttonState;

	SDLKey m_key;
	Uint8 m_joyIndex;
	Uint8 m_joyButton;
	Uint8 m_joyAxis;
};


class Input {

private:
	// Stores the states of keys
	static bool keys[1024];
	static bool keysPressed[1024];
	static bool keysReleased[1024];
	// Stores the states of Joystick axes
	static Sint16 axisValue[128][128]; // [which][axis]
	// Stores the states of Joystick buttons
	static bool joyButtons[128][1024];
	static bool joyButtonsPressed[128][1024];
	static bool joyButtonsReleased[128][1024];

public:
	Input();

	///< Update the state of a key.
	static void updateKey(SDLKey key, bool isDown);

	///< Whether a key is down.
	static bool isKeyDown(SDLKey key);

	///< Whether a key was pressed.
	static bool isKeyPressed(SDLKey key);

	///< Whether a key was released.
	static bool isKeyReleased(SDLKey key);

	///< Update the state of a joy button.
	static void updateJoyButton(Uint8 joystickIndex, Uint8 button, bool isDown);

	///< Whether a joy button is down.
	static bool isJoyButtonDown(Uint8 joystickIndex, Uint8 button);

	///< Whether a joy button was pressed.
	static bool isJoyButtonPressed(Uint8 joystickIndex, Uint8 button);

	///< Whether a joy button was released.
	static bool isJoyButtonReleased(Uint8 joystickIndex, Uint8 button);
	
	///< Update joystick axis value.
	static void updateJoyAxisValue(Uint8 joystickIndex, Uint8 axisIndex, Sint16 value);

	///< Get joystick axis value.
	static double getJoyAxisValue(Uint8 joystickIndex, Uint8 axisIndex);
	
	///< Step input to reset temporary input state.
	static void stepInput();

	// If you might delete base class pointers, you need a virtual destructor.
	virtual ~Input();
};
#endif