#ifndef SAILPLANE_H
#define SAILPLANE_H
#include <zenilib.h>
#include "GameObject.h"

class Player;

class Sailplane : public GameObject {
	friend class Player;
private:
	Player * m_driver;
	double m_wingspan;
	double m_wingdepth;

	double m_airDensity;

	const Zeni::Vector3f getNonLateralVelocity() const;
	const Zeni::Vector3f getLift() const;
	const Zeni::Vector3f getDrag() const;
	const double getLiftCoefficient() const;

	void pitch(double amount);
	void roll(double amount);
	GameObject * fire(double rate);
	void useSpecial();

public:
	Sailplane(const Zeni::Point3f &position = Zeni::Point3f(),
		const Zeni::Quaternion &orientation = Zeni::Quaternion(),
		const Zeni::Model &model = Zeni::Model("models/plane.3ds"),
		const CollisionGeometry &collisionGeometry = CollisionGeometry("models/plane.col"),
		const Zeni::Vector3f &velocity = Zeni::Vector3f(100,0,0),
		const Zeni::Vector3f &force = Zeni::Vector3f(),
		const double mass = 200.0,
		const Zeni::Vector3f &scale = Zeni::Vector3f(10.0f, 10.0f, 10.0f));

	virtual void stepPhysics(const double timeStep);
	virtual void handleCollisions(const std::vector<GameObject*> &collisions);
	virtual const StateModifications act(const std::vector<GameObject*> &collisions);

};
#endif