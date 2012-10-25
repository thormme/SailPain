#ifndef BUILDING_H
#define BUILDING_H
#include <zenilib.h>
#include "GameObject.h"

class Building : public GameObject {
private:

public:
	Building(const Zeni::Point3f &position = Zeni::Point3f(),
		const Zeni::Quaternion &orientation = Zeni::Quaternion(),
		const Zeni::Model &model = Zeni::Model("models/building.3ds"),
		const CollisionGeometry &collisionGeometry = CollisionGeometry("models/building.col"),
		const Zeni::Vector3f &scale = Zeni::Vector3f(100.0f, 100.0f, 100.0f));

};
#endif