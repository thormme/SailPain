#ifndef UTILS_H
#define UTILS_H
#include <zenilib.h>
#include <sstream>

namespace Utils {
	const double PI = 3.141592653589793238462;

	const double getAngleFromVector(const Zeni::Vector2f &vector);
	const Zeni::Vector2f getVectorFromAngle(const double angle);
	const double getAngleDifference(const double angle1, const double angle2);
	const Zeni::Vector3f getVectorComponent(const Zeni::Vector3f &vector, const Zeni::Vector3f &direction);
	const double getAngleBetweenVectors(const Zeni::Vector3f &v0, const Zeni::Vector3f &v1);

	const bool isPointWithinFace(const Zeni::Point3f &point, const Zeni::Point3f &p1, const Zeni::Point3f &p2, const Zeni::Point3f &p3);
	const bool isPointWithinFace(const Zeni::Point3f &point, const Zeni::Point3f vertices[3]);
	const Zeni::Point3f getFaceLineSegmentIntersection(const Zeni::Collision::Line_Segment &lineSegment, const Zeni::Point3f &p1, const Zeni::Point3f &p2, const Zeni::Point3f &p3);
	const Zeni::Point3f getFaceLineSegmentIntersection(const Zeni::Collision::Line_Segment &lineSegment, const Zeni::Point3f vertices[3]);
	const Zeni::Vector3f getSurfaceNormal(const Zeni::Point3f &p1, const Zeni::Point3f &p2, const Zeni::Point3f &p3);
	const Zeni::Vector3f getSurfaceNormal(const Zeni::Point3f vertices[3]);

	template <class T>
	void printDebugMessage(T message) {
		std::ostringstream str;
		str << message;
		OutputDebugString(str.str().c_str());
	}
}
#endif