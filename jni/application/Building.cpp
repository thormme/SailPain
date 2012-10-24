#include <zenilib.h>
#include "Building.h"

Building::Building(const Zeni::Point3f &position,
		const Zeni::Quaternion &orientation,
		const Zeni::Model &model,
		const CollisionGeometry &collisionGeometry,
		const Zeni::Vector3f &scale) : GameObject(position, orientation, model, collisionGeometry, Zeni::Vector3f(0.0f, 0.0f, 0.0f), Zeni::Vector3f(0.0f, 0.0f, 0.0f), 1.0, scale) {
	collideWithGameObjects();
}