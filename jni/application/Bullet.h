#ifndef BULLET_H
#define BULLET_H
#include <zenilib.h>
#include "GameObject.h"

class Bullet : public GameObject {
private:
	GameObject * m_owner;
	double m_deathClock;

public:
	Bullet(GameObject * owner,
		const Zeni::Point3f &position = Zeni::Point3f(),
		const Zeni::Quaternion &orientation = Zeni::Quaternion(),
		const Zeni::Vector3f &velocity = Zeni::Vector3f());

	virtual const StateModifications act(const std::vector<GameObject*> &collisions);
};
#endif