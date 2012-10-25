#include <zenilib.h>
#include "Bullet.h"

Bullet::Bullet(GameObject * owner,
		const Zeni::Point3f &position,
		const Zeni::Quaternion &orientation,
		const Zeni::Vector3f &velocity) : GameObject(position, orientation, Zeni::Model("models/bullet.3ds"), CollisionGeometry("models/bullet.col"), velocity, Zeni::Vector3f(0.0f, 0.0f, 0.0f), 1.0, Zeni::Vector3f(20.0f, 3.0f, 3.0f)) {
	detectCollisionsWithGameObjects();
}