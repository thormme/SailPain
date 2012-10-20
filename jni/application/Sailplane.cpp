#include <zenilib.h>
#include "Sailplane.h"
#include "Utils.h"
#include "Input.h"

Sailplane::Sailplane(const Zeni::Point3f &position,
		const Zeni::Quaternion &orientation,
		const Zeni::Model &model,
		const Zeni::Vector3f &velocity,
		const Zeni::Vector3f &force,
		const double &mass) : GameObject(position, orientation, model, velocity, force, mass) {
	m_wingspan = 15.0;
	m_wingdepth = 2.0;
}

const Zeni::Vector3f Sailplane::getDrag() const {
	return Zeni::Vector3f();
}

const Zeni::Vector3f Sailplane::getLift() const {
	// get non-lateral velocity
	Zeni::Vector3f velocity = getVelocity() - Utils::getVectorComponent(getVelocity(), Zeni::Quaternion(Utils::PI/2.0, 0.0f, 0.0f) * getVelocity());
	return velocity;
}

const double Sailplane::getLiftCoefficient() const {
	Zeni::Vector3f velocity = getVelocity() - Utils::getVectorComponent(getVelocity(), Zeni::Quaternion(Utils::PI/2.0, 0.0f, 0.0f) * getForwardVector());
	double angleOfAttack = 0.0;
	if (velocity.magnitude() != 0) {
		angleOfAttack = Utils::getQuaternionPitch(Zeni::Quaternion(3, 2, 1));
	}
	return angleOfAttack;
}

void Sailplane::stepPhysics(const double timeStep) {
	double halfLife = .1;
	setYawRate(getYawRate()*pow(0.5, timeStep/halfLife));
	setPitchRate(getPitchRate()*pow(0.5, timeStep/halfLife));
	setRollRate(getRollRate()*pow(0.5, timeStep/halfLife));
	//setForce(getForce() + getDrag() + getLift());
	Utils::printDebugMessage(getLiftCoefficient());
	Utils::printDebugMessage("\n");
	GameObject::stepPhysics(timeStep);
}

void Sailplane::handleCollisions(const std::vector<GameObject*> &collisions) {
	for (int i=0; i < collisions.size(); i++) {
		Utils::printDebugMessage("hi!\n");
	}
}

const StateModifications Sailplane::act(const std::vector<GameObject*> &collisions) {
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
		setForce(-getForwardVector()*10);
	}
	if (Input::isKeyDown(SDLK_a)) {
		setRollRate(-Utils::PI/2.0);
	}
	if (Input::isKeyDown(SDLK_d)) {
		setRollRate(Utils::PI/2.0);
	}
	if (Input::isKeyDown(SDLK_w)) {
		setPitchRate(Utils::PI/2.0);
	}
	if (Input::isKeyDown(SDLK_s)) {
		setPitchRate(-Utils::PI/2.0);
	}
	return StateModifications();
}