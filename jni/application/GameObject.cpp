#include <zenilib.h>
#include "GameObject.h"

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
		m_force(force) {
	m_collideWithGameObjects = false;
	m_yawRate = 0.0;
	m_pitchRate = 0.0;
	m_rollRate = 0.0;
}

const bool GameObject::isTouching(GameObject* object) const {
	return false;
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

const Zeni::Quaternion GameObject::getOrientation() const {
	return m_orientation;
}

void GameObject::setOrientation(Zeni::Quaternion orientation) {
	m_orientation = orientation;
	m_model.set_rotate(m_orientation);
}

const bool GameObject::willDetectCollisionsWithGameObjects() {
	return m_collideWithGameObjects;
}