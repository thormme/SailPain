#include <zenilib.h>
#include "Player.h"
#include "Input.h"
#include "Utils.h"

Player::Player() {
	m_controls.resize(FINAL_ELEMENT, InputAction());
	m_plane = nullptr;
	m_planeModel = "";
}

Sailplane * Player::getNewPlane(const Zeni::Point3f &position, const Zeni::Quaternion orientation) {
	if (m_planeModel != "") {
		m_plane = new Sailplane(position, orientation, Zeni::Model(m_planeModel));
	} else {
		m_plane = new Sailplane(position, orientation);
	}
	m_plane->m_driver = this;
	return m_plane;
}

Sailplane * Player::getLastPlane() {
	return m_plane;
}

void Player::setControls(const std::vector<InputAction> &controls) {
	m_controls = controls;
	m_controls.resize(FINAL_ELEMENT, InputAction());
}

void Player::setControls(int joyIndex) {
	if (joyIndex >= 0) {
		m_controls[FORWARD] = InputAction(joyIndex, Zeni::Joysticks::AXIS_LEFT_THUMB_Y, -2.0, 0.25);
		m_controls[BACKWARD] = InputAction(joyIndex, Zeni::Joysticks::AXIS_LEFT_THUMB_Y, -0.25, 2.0);
		m_controls[LEFT] = InputAction(joyIndex, Zeni::Joysticks::AXIS_LEFT_THUMB_X, -0.25, 2.0);
		m_controls[RIGHT] = InputAction(joyIndex, Zeni::Joysticks::AXIS_LEFT_THUMB_X, -2.0, 0.25);
		m_controls[FIRE] = InputAction(joyIndex, Zeni::Joysticks::AXIS_RIGHT_TRIGGER, -2.0, 0.25);
		m_controls[SPECIAL] = InputAction(joyIndex, Zeni::Joysticks::AXIS_LEFT_TRIGGER, -2.0, 0.25);
	}  else {
		m_controls[FORWARD] = InputAction(SDLK_UP);
		m_controls[BACKWARD] = InputAction(SDLK_DOWN);
		m_controls[LEFT] = InputAction(SDLK_LEFT);
		m_controls[RIGHT] = InputAction(SDLK_RIGHT);
		m_controls[FIRE] = InputAction(SDLK_q);
		m_controls[SPECIAL] = InputAction(SDLK_w, InputAction::ButtonState::PRESSED);
	}
}

void Player::setPlaneModel(const Zeni::String &model) {
	m_planeModel = model;
}

const Zeni::String Player::getPlaneModel() const {
	return m_planeModel;
}

const StateModifications Player::drivePlane(Sailplane &sailplane, const std::vector<GameObject*> &gameObjectCollisions) {
	StateModifications stateModifications = StateModifications();
	if (m_controls[FORWARD].isActive()) {
		sailplane.pitch(abs(m_controls[FORWARD].getDegree()));
	}
	
	if (m_controls[BACKWARD].isActive()) {
		sailplane.pitch(-abs(m_controls[BACKWARD].getDegree()));
	}

	if (m_controls[LEFT].isActive()) {
		sailplane.roll(-abs(m_controls[LEFT].getDegree()));
	}

	if (m_controls[RIGHT].isActive()) {
		sailplane.roll(abs(m_controls[RIGHT].getDegree()));
	}

	if (m_controls[FIRE].isActive()) {
		GameObject * object = sailplane.fire(m_controls[FIRE].getDegree());
		if (object != nullptr) {
			stateModifications.gameObjectAdditions.push_back(object);
		}
	}

	if (m_controls[SPECIAL].isActive()) {
		sailplane.useSpecial();
	}

	/*if (Input::isKeyDown(SDLK_q)) {
		for (std::vector<Tile*>::const_iterator it = tileCollisions.begin(); it != tileCollisions.end(); it++) {
			Tile newTile = Tile((*it)->getPosition(), (*it)->getSize(), Zeni::String("grass"));
			stateModifications.tileChanges.push_back(newTile);
		}
	}*/
	return stateModifications;
}