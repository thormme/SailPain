#ifndef COLLISIONGEOMETRY_H
#define COLLISIONGEOMETRY_H
#include <zenilib.h>

class CollisionGeometry {
private:
	std::vector<Zeni::Collision::Parallelepiped> m_geometry;

public:
	CollisionGeometry(std::string fileName = "models/placeholder.col");

	const bool isTouching(const CollisionGeometry &collisionGeometry, 
		const Zeni::Point3f &position1 = Zeni::Vector3f(), 
		const Zeni::Point3f &position2 = Zeni::Vector3f(), 
		const Zeni::Quaternion &orientation1 = Zeni::Quaternion(),
		const Zeni::Quaternion &orientation2 = Zeni::Quaternion(),
		const Zeni::Vector3f &scale1 = Zeni::Vector3f(1.0f, 1.0f, 1.0f),
		const Zeni::Vector3f &scale2 = Zeni::Vector3f(1.0f, 1.0f, 1.0f)) const;

	const bool CollisionGeometry::isTouching(const Zeni::Collision::Plane &plane, 
		const Zeni::Point3f &position1,
		const Zeni::Quaternion &orientation1,
		const Zeni::Vector3f &scale1) const;
};
#endif