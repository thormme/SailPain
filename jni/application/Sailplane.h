#ifndef SAILPLANE_H
#define SAILPLANE_H
#include <zenilib.h>
#include "GameObject.h"
#include "Powerup.h"

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

	struct Stats {
		int kills;
		int deaths;
	};

	Sailplane(const Zeni::Point3f &position = Zeni::Point3f(),
		const Zeni::Quaternion &orientation = Zeni::Quaternion(),
		const Zeni::Model &model = Zeni::Model("models/plane.3ds"),
		const CollisionGeometry &collisionGeometry = CollisionGeometry("models/plane.col"),
		const Zeni::Vector3f &velocity = Zeni::Vector3f(100,0,0),
		const Zeni::Vector3f &force = Zeni::Vector3f(),
		const double mass = 400.0,
		const Zeni::Vector3f &scale = Zeni::Vector3f(10.0f, 10.0f, 10.0f));

	virtual void stepPhysics(const double timeStep);
	virtual void handleCollisions(const std::vector<GameObject*> &collisions);
	virtual const StateModifications act(const std::vector<GameObject*> &collisions);

	bool damage(int amount); ///< Returns true if the damage destroyed the plane.
	void incrementKills();
	const Consumable getHealth() const;
	const Stats getStats() const;

private:
	Player * m_driver;
	Consumable m_health;
	bool m_disabled;
	bool m_respawning;
	double m_respawnTimer;
	double m_fireTimer;
	bool m_hasPowerup;
	bool m_boosting;
	double m_boostingTimer;
	Powerup::Type m_powerup;
	Zeni::Point3f m_initialPosition;
	Zeni::Vector3f m_initialVelocity;
	Zeni::Quaternion m_initialOrientation;
	int m_kills;
	int m_deaths;

	const Zeni::Vector3f getNonLateralVelocity() const;
	const Zeni::Vector3f getLift() const;
	const Zeni::Vector3f getDrag() const;
	const double getLiftCoefficient() const;
	const double getTailYawCorrection() const;

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