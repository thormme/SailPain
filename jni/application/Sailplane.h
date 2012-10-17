#ifndef SAILPLANE_H
#define SAILPLANE_H
#include <zenilib.h>
#include "GameObject.h"

class Sailplane : public GameObject {
private:

public:
	Sailplane(const Zeni::Point3f &position = Zeni::Point3f(),
		const Zeni::Quaternion &orientation = Zeni::Quaternion(),
		const Zeni::Model &model = Zeni::Model("models/placeholder.3ds"),
		const Zeni::Vector3f &velocity = Zeni::Vector3f(),
		const Zeni::Vector3f &force = Zeni::Vector3f(),
		const double &mass = 1.0);

	virtual void handleCollisions(const std::vector<GameObject*> &collisions);
	virtual const StateModifications act(const std::vector<GameObject*> &collisions);

};
#endif