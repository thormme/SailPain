#include <zenilib.h>
#include "GameObject.h"
#include "Input.h"
#include "Utils.h"
#include "lib3ds.h"

GameObject::GameObject(const Zeni::Point3f &position,
		const Zeni::Quaternion &orientation,
		const Zeni::Model &model,
		const Zeni::Vector3f &velocity,
		const Zeni::Vector3f &force,
		const double &mass)
		: m_position(position),
		m_orientation(orientation),
		m_model(model),
		m_velocity(velocity),
		m_force(force),
		m_mass(mass) {
	m_collideWithGameObjects = false;
	m_yawRate = 0.0;
	m_pitchRate = 0.0;
	m_rollRate = 0.0;
}

const bool GameObject::isTouching(const GameObject* object) const {
	Lib3dsFile * file = m_model.get_file();

	for(int i=0; i < file->nmeshes; i++) {
		for(int j=0; j < file->meshes[i]->nfaces; j++) {
			Utils::printDebugMessage(file->meshes[i]->faces[j].flags);
			Utils::printDebugMessage("\n");
		}
	}
	return object->isTouching(this);
}

void GameObject::stepPhysics(const double timeStep) {
	setOrientation(getOrientation() * Zeni::Quaternion(m_yawRate * timeStep, m_pitchRate * timeStep, m_rollRate * timeStep));
	Zeni::Vector3f acceleration = m_force / m_mass;
	m_velocity += acceleration * timeStep;
	setPosition(getPosition() + m_velocity * timeStep);
}

void GameObject::handleCollisions(const std::vector<GameObject*> &collisions) {

}

const StateModifications GameObject::act(const std::vector<GameObject*> &collisions) {
	setYawRate(0.0);
	setPitchRate(0.0);
	setRollRate(0.0);
	setForce(Zeni::Vector3f());
	if (Input::isKeyDown(SDLK_LEFT)) {
		setYawRate(Utils::PI/2.0);
	}
	if (Input::isKeyDown(SDLK_RIGHT)) {
		setYawRate(-Utils::PI/2.0);
	}
	if (Input::isKeyDown(SDLK_UP)) {
		setForce(getForwardVector()*10);
	}
	if (Input::isKeyDown(SDLK_DOWN)) {
		setForce(getForwardVector()*10);
	}
	return StateModifications();
}

void GameObject::render() {
	m_model.set_translate(m_position);
	m_model.set_rotate(m_orientation);
	m_model.render();
}

const Zeni::Point3f GameObject::getPosition() const {
	return m_position;
}

void GameObject::setPosition(Zeni::Point3f position) {
	m_position = position;
	m_model.set_translate(m_position);
}

const Zeni::Vector3f GameObject::getVelocity() const {
	return m_velocity;
}

void GameObject::setVelocity(Zeni::Vector3f velocity) {
	m_velocity = velocity;
}

const Zeni::Vector3f GameObject::getForce() const {
	return m_force;
}

void GameObject::setForce(Zeni::Vector3f force) {
	m_force = force;
}

const Zeni::Quaternion GameObject::getOrientation() const {
	return m_orientation;
}

const Zeni::Vector3f GameObject::getForwardVector() const {
	return (m_orientation*Zeni::Vector3f(1.0f, 0.0f, 0.0f)).normalize();
}

void GameObject::setOrientation(Zeni::Quaternion orientation) {
	m_orientation = orientation;
	m_model.set_rotate(m_orientation);
}

const double GameObject::getYawRate() const {
	return m_yawRate;
}

void GameObject::setYawRate(double rate) {
	m_yawRate = rate;
}

const double GameObject::getPitchRate() const {
	return m_pitchRate;
}

void GameObject::setPitchRate(double rate) {
	m_pitchRate = rate;
}

const double GameObject::getRollRate() const {
	return m_rollRate;
}

void GameObject::setRollRate(double rate) {
	m_rollRate = rate;
}

const bool GameObject::willDetectCollisionsWithGameObjects() {
	return m_collideWithGameObjects;
}