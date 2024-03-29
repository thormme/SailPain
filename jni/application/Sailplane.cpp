#include <zenilib.h>
#include "Sailplane.h"
#include "Player.h"
#include "Utils.h"
#include "Input.h"
#include "Bullet.h"
#include "Time.h"

Sailplane::Sailplane(const Zeni::Point3f &position,
		const Zeni::Quaternion &orientation,
		const Zeni::Model &model,
		const CollisionGeometry &collisionGeometry,
		const Zeni::Vector3f &velocity,
		const Zeni::Vector3f &force,
		const double mass,
		const Zeni::Vector3f &scale) : GameObject(position, orientation, model, collisionGeometry, velocity, force, mass, scale) {
	m_health.max = 10;
	m_health.refill();
	m_driver = nullptr;
	m_respawning = false;
	m_hasPowerup = false;
	m_boosting = false;
	m_respawnTimer = 0.0;
	m_fireTimer = 0.0;
	m_kills = 0;
	m_deaths = 0;
	setDisabled(false);
	m_initialPosition = getPosition();
	m_initialVelocity = getVelocity();
	m_initialOrientation = getOrientation();
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
		angleOfAttack += Utils::PI/42.0;
	}
	double liftCoefficient = -pow((abs(angleOfAttack)-(19.0/180*Utils::PI))/(-19.0/180*Utils::PI), 2.0) + 1.0;
	if (liftCoefficient < 0.0) {
		liftCoefficient = 0.0;
	} else if (angleOfAttack < 0.0) {
		liftCoefficient *= -1;
	}
	return liftCoefficient;
}

const double Sailplane::getTailYawCorrection() const {
	Zeni::Quaternion up = getOrientation() * Zeni::Quaternion(0.0, Utils::PI/2.0, 0.0);
	Zeni::Vector3f upVector = (up*Zeni::Vector3f(1.0f, 0.0f, 0.0f)).normalize();
	Zeni::Vector3f velocity = getVelocity() - Utils::getVectorComponent(getVelocity(), upVector);
	double angleOfAttack = 0.0;
	if (velocity.magnitude() != 0) {
		angleOfAttack = Utils::getAngleBetweenVectors(velocity, getForwardVector());

		Zeni::Quaternion positive = getOrientation() * Zeni::Quaternion(angleOfAttack, 0.0f, 0.0f);
		Zeni::Vector3f positiveVector = (positive*Zeni::Vector3f(1.0f, 0.0f, 0.0f)).normalize();
		Zeni::Quaternion negative = getOrientation() * Zeni::Quaternion(-angleOfAttack, 0.0f, 0.0f);
		Zeni::Vector3f negativeVector = (negative*Zeni::Vector3f(1.0f, 0.0f, 0.0f)).normalize();

		if (Utils::getAngleBetweenVectors(positiveVector, velocity) > Utils::getAngleBetweenVectors(negativeVector, velocity)) {
			angleOfAttack *= -1;
		}
	}
	return angleOfAttack * velocity.magnitude()/100.0f;
}

void Sailplane::pitch(double amount) {
	setPitchRate(Utils::PI/2.0 * amount);
}

void Sailplane::roll(double amount) {
	setRollRate(Utils::PI/2.0 * amount);
}

GameObject * Sailplane::fire(double rate) {
	if (Time::getGameTime() - m_fireTimer > 0.2) {
		m_fireTimer = Time::getGameTime();
		return new Bullet(this, getPosition(), getOrientation(), getForwardVector() * 1500.0f + Utils::getVectorComponent(getVelocity(), getForwardVector()));
	} else {
		return nullptr;
	}
}

void Sailplane::useSpecial() {
	if (m_hasPowerup) {
		m_hasPowerup = false;
		switch (m_powerup) {
		case Powerup::Type::BOOST:
			m_boosting = true;
			m_boostingTimer = Time::getGameTime();
			break;
		case Powerup::Type::MISSILE:
			//return Zeni::Model("models/powerup_missile.3ds");
			break;
		case Powerup::Type::HEALTH:
			m_health.refill();
			break;
		default:
			break;
		}
	}
}

void Sailplane::stepPhysics(const double timeStep) {
	double halfLife = .1;
	setYawRate(getYawRate()*pow(0.5, timeStep/halfLife) + getTailYawCorrection());
	setPitchRate(getPitchRate()*pow(0.5, timeStep/halfLife));
	setRollRate(getRollRate()*pow(0.5, timeStep/halfLife));
	setForce(getForce() + getDrag() + getLift() + Zeni::Vector3f(0.0, 0.0, -10.0) * getMass());
	GameObject::stepPhysics(timeStep);
}

void Sailplane::handleCollisions(const std::vector<GameObject*> &collisions) {
	
}

const StateModifications Sailplane::act(const std::vector<GameObject*> &collisions) {
	StateModifications stateModifications;

	setForce(Zeni::Vector3f());

	for (int i=0; i < collisions.size(); i++) {
		if (collisions[i]->isSolid()) {
			m_health.consume(m_health.remaining);
		}
		Powerup * powerup = dynamic_cast<Powerup*>(collisions[i]);
		if (powerup != nullptr && m_hasPowerup == false) {
			m_powerup = powerup->getType();
			stateModifications.gameObjectRemovals.push_back(powerup);
			m_hasPowerup = true;
		}
	}

	if (m_driver != nullptr && !m_disabled) {
		stateModifications.combine(m_driver->drivePlane(*this, collisions));
	}
	if (m_health.remaining <= 0) {
		respawn();
		//setImage("race_car-destroyed");
	}
	if (m_disabled == true) {
		/*setBraking(true);
		accelerate(0);*/
	}
	if (m_respawning == true) {
		if (Time::getGameTime() - m_respawnTimer > 2.0) {
			completeRespawn();
		}
	}
	if (m_boosting == true) {
		setForce(getForce() + getForwardVector()*50000);
		if (Time::getGameTime() - m_boostingTimer > 2.0) {
			m_boosting = false;
		}
	}

	if (Input::isKeyDown(SDLK_a)) {
		setForce(getForwardVector()*10000);
	}
	if (Input::isKeyDown(SDLK_s)) {
		setForce(-getForwardVector()*10000);
	}

	return stateModifications;
}

// Returns true if the damage destroyed the plane.
bool Sailplane::damage(int amount) {
	int beginHealth = m_health.remaining;
	m_health.consume(amount);
	if (m_health.remaining == 0 && beginHealth > 0) {
		m_deaths++;
		return true;
	}
	return false;
}

void Sailplane::incrementKills() {
	m_kills++;
}

const Sailplane::Consumable Sailplane::getHealth() const {
	return m_health;
}

const Sailplane::Stats Sailplane::getStats() const {
	Stats stats;
	stats.deaths = m_deaths;
	stats.kills = m_kills;
	return stats;
}

void Sailplane::setDisabled(bool disabled) {
	m_disabled = disabled;
	detectCollisionsWithGameObjects(!disabled);
	collideWithGameObjects(!disabled);
}

void Sailplane::respawn() {
	if (m_respawning == false) {
		setDisabled(true);
		m_respawning = true;
		m_respawnTimer = Time::getGameTime();
	}
}

void Sailplane::completeRespawn() {
	setYawRate(0.0);
	setPitchRate(0.0);
	setRollRate(0.0);
	setVelocity(m_initialVelocity);
	setPosition(m_initialPosition);
	setOrientation(m_initialOrientation);
	m_health.refill();
	//setImage(m_originalImage);
	setDisabled(false);
	m_respawning = false;
}