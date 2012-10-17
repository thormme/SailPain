#include <zenilib.h>
#include <cmath>
#include <sstream>
#include "Utils.h"

const double Utils::getAngleFromVector(const Zeni::Vector2f &vector) {
	if (vector.i == 0.0) return PI/2.0;
	double angle = atan(vector.j / vector.i);
	if (vector.i < 0) angle += PI;
	return angle;
}

const Zeni::Vector2f Utils::getVectorFromAngle(const double angle) {
	return Zeni::Vector2f(cos(angle), sin(angle));
}

const double Utils::getAngleDifference(const double angle1, const double angle2) {
	double a1 = std::fmod(angle1, PI*2.0);
	if (a1 < 0) {
		a1 += Utils::PI*2.0;
	}
	double a2 = std::fmod(angle2, PI*2.0);
	if (a2 < 0) {
		a2 += Utils::PI*2.0;
	}
	double difference = std::fmod(a2, PI*2.0) - std::fmod(a1, PI*2.0);
	if (difference > PI) {
		return difference - PI*2.0;
	}
	if (difference < -PI) {
		return difference + PI*2.0;
	}
	return difference;
}

const bool Utils::isPointWithinFace(const Zeni::Point3f &point, const Zeni::Point3f &p1, const Zeni::Point3f &p2, const Zeni::Point3f &p3) {
	Zeni::Vector3f v1 = p3 - p1;
	Zeni::Vector3f v2 = p2 - p1;
	Zeni::Vector3f v3 = point - p1;

	double dot11 = v1*v1;
	double dot12 = v1*v2;
	double dot13 = v1*v3;
	double dot22 = v2*v2;
	double dot23 = v2*v3;

	// Barycentricize!!
	double invDenom = 1.0/(dot11 * dot22 - dot12 * dot12);
	double u = (dot22 * dot13 - dot12 * dot23) * invDenom;
	double v = (dot11 * dot23 - dot12 * dot13) * invDenom;

	if (u >= 0.0f && v >= 0.0f && u + v < 1) {
		return true;
	}
	return false;
}

const Zeni::Point3f Utils::getFaceLineSegmentIntersection(const Zeni::Collision::Line_Segment &lineSegment, const Zeni::Point3f &p1, const Zeni::Point3f &p2, const Zeni::Point3f &p3) {
	// Interpolate along line segment to the interpolation value returned by the collision
	Zeni::Collision::Plane facePlane(p1, getSurfaceNormal(p1, p2, p3));
	return lineSegment.get_end_point_a().interpolate_to(lineSegment.nearest_point(facePlane).second, lineSegment.get_end_point_b());
}

const Zeni::Vector3f Utils::getSurfaceNormal(const Zeni::Point3f &p1, const Zeni::Point3f &p2, const Zeni::Point3f &p3) {
	Zeni::Vector3f v1 = p2 - p1;
	Zeni::Vector3f v2 = p3 - p2;
	Zeni::Vector3f normal = Zeni::Vector3f();
	normal.i = (v1.y * v2.z) - (v1.z * v2.y);
	normal.j = (v1.z * v2.x) - (v1.x * v2.z);
	normal.k = (v1.x * v2.y) - (v1.y * v2.x);
	return normal.normalize();
}

const bool Utils::isPointWithinFace(const Zeni::Point3f &point, const Zeni::Point3f vertices[3]) {
	return isPointWithinFace(point, vertices[0], vertices[1], vertices[2]);
}

const Zeni::Vector3f Utils::getSurfaceNormal(const Zeni::Point3f vertices[3]) {
	return getSurfaceNormal(vertices[0], vertices[1], vertices[2]);
}

const Zeni::Point3f Utils::getFaceLineSegmentIntersection(const Zeni::Collision::Line_Segment &lineSegment, const Zeni::Point3f vertices[3]) {
	return getFaceLineSegmentIntersection(lineSegment, vertices[0], vertices[1], vertices[2]);
}