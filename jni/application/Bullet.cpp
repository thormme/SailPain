#include <zenilib.h>
#include "Bullet.h"
#include "Time.h"
#include "Sailplane.h"

Bullet::Bullet(GameObject * owner,
		const Zeni::Point3f &position,
		const Zeni::Quaternion &orientation,
		const Zeni::Vector3f &velocity) : m_owner(owner), GameObject(position, orientation, Zeni::Model("models/bullet.3ds"), CollisionGeometry("models/bullet.col"), velocity, Zeni::Vector3f(0.0f, 0.0f, 0.0f), 1.0, Zeni::Vector3f(20.0f, 3.0f, 3.0f)) {
	detectCollisionsWithGameObjects();
	m_deathClock = Time::getGameTime();
}

const StateModifications Bullet::act(const std::vector<GameObject*> &collisions) {
	StateModifications stateModifications;
	for (int i=0; i < collisions.size(); i++) {
		if (collisions[i] != m_owner) {
			Sailplane * plane = dynamic_cast<Sailplane*>(collisions[i]);
			if (plane != nullptr) {
				stateModifications.gameObjectRemovals.push_back(this);
				if (plane->damage(1)) {
					Sailplane * ownerPlane = dynamic_cast<Sailplane*>(m_owner);
					if (ownerPlane != nullptr) {
						ownerPlane->incrementKills();
					}
				}
				break;
			} else {
				Bullet * bullet = dynamic_cast<Bullet*>(collisions[i]);
				if (bullet == nullptr) {
					stateModifications.gameObjectRemovals.push_back(this);
				}
			}
		}
	}
	if (Time::getGameTime() - m_deathClock > 10.0 && stateModifications.gameObjectRemovals.size() == 0) {
		stateModifications.gameObjectRemovals.push_back(this);
	}

	return stateModifications;
}