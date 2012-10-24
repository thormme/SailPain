#include <zenilib.h>
#include "Sailplane.h"
#include "Utils.h"
#include "Input.h"

Sailplane::Sailplane(const Zeni::Point3f &position,
		const Zeni::Quaternion &orientation,
		const Zeni::Model &model,
		const CollisionGeometry &collisionGeometry,
		const Zeni::Vector3f &velocity,
		const Zeni::Vector3f &force,
		const double mass,
		const Zeni::Vector3f &scale) : GameObject(position, orientation, model, collisionGeometry, velocity, force, mass, scale) {
	m_wingspan = 15.0;
	m_wingdepth = 2.0;
	m_airDensity = 1.0;
	detectCollisionsWithGameObjects();
	collideWithGameObjects();
}

const Zeni::Vector3f Sailplane::getNonLateralVelocity() const {
	Zeni::Quaternion left = getOrientation() * Zeni::Quaternion(Utils::PI/2.0, 0.0, 00.0);
	Zeni::Vector3f leftVector = (left*Zeni::Vector3f(1.0f, 0.0f, 0.0f)).normalize();
	return getVelocity() - Utils::getVectorComponent(getVelocity(), leftVector);
}

const Zeni::Vector3f Sailplane::getDrag() const {
	/*double baseDrag = 0.01812;
	double liftCoefficient = abs(getLiftCoefficient());
	double efficiency = 1.0;
	double wingArea = m_wingspan * m_wingdepth;
	double dragCoefficient = baseDrag + liftCoefficient*liftCoefficient / ( Utils::PI * m_wingspan*m_wingspan/wingArea * efficiency);
	Zeni::Vector3f velocity = getNonLateralVelocity();
	double drag = dragCoefficient * m_airDensity * velocity.magnitude2() * 0.5 * wingArea;
	//return -velocity.normalized()*drag;
	return Zeni::Vector3f();*/
	Zeni::Vector3f forwardVector = getForwardVector();
	Zeni::Vector3f nonForwardVelocity = getVelocity() - Utils::getVectorComponent(getVelocity(), forwardVector);

	Zeni::Quaternion left = getOrientation() * Zeni::Quaternion(Utils::PI/2.0, 0.0, 00.0);
	Zeni::Vector3f leftVector = (left*Zeni::Vector3f(1.0f, 0.0f, 0.0f)).normalize();
	Zeni::Vector3f nonForwardSideVelocity = Utils::getVectorComponent(nonForwardVelocity, leftVector);

	Zeni::Vector3f velocity = nonForwardVelocity - nonForwardSideVelocity + nonForwardSideVelocity*2.0;

	return -velocity * getMass() * .1;
}

const Zeni::Vector3f Sailplane::getLift() const {

	// get non-lateral velocity
	Zeni::Vector3f velocity = getNonLateralVelocity();
	//Zeni::Vector3f upVector = (Zeni::Quaternion(0.0, Utils::PI/2.0, 0.0) * velocity).normalize();
	Zeni::Quaternion up = getOrientation() * Zeni::Quaternion(0.0, -Utils::PI/2.0, 00.0);
	Zeni::Vector3f upVector = (up*Zeni::Vector3f(1.0f, 0.0f, 0.0f)).normalize();

	//double lift = 0.5 * m_airDensity * velocity.magnitude2() * m_wingspan * m_wingdepth * getLiftCoefficient();
	double lift = getLiftCoefficient() * Utils::getVectorComponent(getVelocity(), getForwardVector()).magnitude2();

	return upVector * lift * getMass() * .01;
}

const double Sailplane::getLiftCoefficient() const {
	Zeni::Vector3f velocity = getNonLateralVelocity();
	double angleOfAttack = 0.0;
	if (velocity.magnitude() != 0) {
		angleOfAttack = Utils::getAngleBetweenVectors(velocity, getForwardVector());

		Zeni::Quaternion slightlyUp = getOrientation() * Zeni::Quaternion(0.0f, angleOfAttack, 0);
		Zeni::Vector3f slightlyUpForwardVector = (slightlyUp*Zeni::Vector3f(1.0f, 0.0f, 0.0f)).normalize();

		if (Utils::getAngleBetweenVectors(slightlyUpForwardVector, velocity) > Utils::getAngleBetweenVectors(slightlyUpForwardVector, getForwardVector())) {
			angleOfAttack *= -1;
		}
	}
	double liftCoefficient = -pow((abs(angleOfAttack)-(19.0/180*Utils::PI))/(-19.0/180*Utils::PI), 2.0) + 1.0;
	if (liftCoefficient < 0.0) {
		liftCoefficient = 0.0;
	} else if (angleOfAttack < 0.0) {
		liftCoefficient *= -1;
	}
	return liftCoefficient;
}

void Sailplane::stepPhysics(const double timeStep) {
	double halfLife = .1;
	setYawRate(getYawRate()*pow(0.5, timeStep/halfLife));
	setPitchRate(getPitchRate()*pow(0.5, timeStep/halfLife));
	setRollRate(getRollRate()*pow(0.5, timeStep/halfLife));
	setForce(getForce() + getDrag() + getLift() + Zeni::Vector3f(0.0, 0.0, -10.0) * getMass());
	Utils::printDebugMessage(getLift().magnitude());
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
		setForce(getForwardVector()*10000);
	}
	if (Input::isKeyDown(SDLK_DOWN)) {
		setForce(-getForwardVector()*10000);
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