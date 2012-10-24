#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <zenilib.h>
#include "CollisionGeometry.h"

class GameObject;

struct StateModifications {
	std::list<GameObject*> gameObjectAdditions;
	std::list<GameObject*> gameObjectRemovals;

	void combine(const StateModifications &stateModifications) {
		gameObjectAdditions.insert(gameObjectAdditions.end(), stateModifications.gameObjectAdditions.begin(), stateModifications.gameObjectAdditions.end());
		gameObjectRemovals.insert(gameObjectRemovals.end(), stateModifications.gameObjectRemovals.begin(), stateModifications.gameObjectRemovals.end());
	}
};

class GameObject {
private:
	Zeni::Model m_model;
	CollisionGeometry m_collisionGeometry;

	Zeni::Point3f m_position;
	Zeni::Vector3f m_velocity;
	Zeni::Vector3f m_force;
	double m_mass;
	Zeni::Vector3f m_scale;
	double m_yawRate;
	double m_pitchRate;
	double m_rollRate;
	Zeni::Quaternion m_orientation;
	bool m_collideWithGameObjects;
	bool m_detectCollisionsWithGameObjects;

protected:
	void detectCollisionsWithGameObjects(bool detect = true);
	void collideWithGameObjects(bool collide = true);

public:
	GameObject(const Zeni::Point3f &position = Zeni::Point3f(),
		const Zeni::Quaternion &orientation = Zeni::Quaternion(),
		const Zeni::Model &model = Zeni::Model("models/placeholder.3ds"),
		const CollisionGeometry &collisionGeometry = CollisionGeometry("models/placeholder.col"),
		const Zeni::Vector3f &velocity = Zeni::Vector3f(),
		const Zeni::Vector3f &force = Zeni::Vector3f(),
		const double mass = 1.0,
		const Zeni::Vector3f &scale = Zeni::Vector3f(1.0f, 1.0f, 1.0f));

	const bool isTouching(GameObject* object);

	virtual void stepPhysics(const double timeStep);
	virtual void handleCollisions(const std::vector<GameObject*> &collisions);
	virtual const StateModifications act(const std::vector<GameObject*> &collisions);

	void render();

	void setPosition(Zeni::Point3f position);
	void setVelocity(Zeni::Vector3f velocity);
	void setForce(Zeni::Vector3f force);
	void setOrientation(Zeni::Quaternion orientation);
	void setRollRate(double rate);
	void setPitchRate(double rate);
	void setYawRate(double rate);

	const Zeni::Point3f getPosition() const;
	const Zeni::Vector3f getVelocity() const;
	const Zeni::Vector3f getForce() const;
	const Zeni::Quaternion getOrientation() const;
	const Zeni::Vector3f getForwardVector() const;
	const double getYawRate() const;
	const double getPitchRate() const;
	const double getRollRate() const;
	const double getMass() const;
	const bool willDetectCollisionsWithGameObjects() const;
	const bool willCollideWithGameObjects() const;
};
#endif