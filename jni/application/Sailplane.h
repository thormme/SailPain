#ifndef SAILPLANE_H
#define SAILPLANE_H
#include <zenilib.h>
#include "GameObject.h"

class Player;

class Sailplane : public GameObject {
	friend class Player;

public:
	struct Consumable {
		int max;
		int remaining;
		Consumable(int max = 0, int remaining = -1) : max(max), remaining(remaining) {
			if (remaining == -1) {
				remaining = max;
			}
		}
		void refill() {
			remaining = max;
		}
		int consume(int amount = 1) {
			amount = std::min(amount, remaining);
			remaining -= amount;
			return amount;
		}
	};

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

	void damage(int amount);
	const Consumable getHealth() const;

private:
	Player * m_driver;
	Consumable m_health;
	bool m_disabled;
	bool m_respawning;
	double m_respawnTimer;
	double m_fireTimer;
	Zeni::Point3f m_initialPosition;
	Zeni::Vector3f m_initialVelocity;
	Zeni::Quaternion m_initialOrientation;

	const Zeni::Vector3f getNonLateralVelocity() const;
	const Zeni::Vector3f getLift() const;
	const Zeni::Vector3f getDrag() const;
	const double getLiftCoefficient() const;

	void pitch(double amount);
	void roll(double amount);
	GameObject * fire(double rate);
	void useSpecial();

protected:
	void setDisabled(bool disabled);
	void respawn();
	void completeRespawn();

};
#endif