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