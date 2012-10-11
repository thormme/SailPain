#ifndef UTILS_H
#define UTILS_H
#include <zenilib.h>
#include <sstream>

namespace Utils {
	const double PI = 3.141592653589793238462;

	const double getAngleFromVector(const Zeni::Vector2f &vector);
	const Zeni::Vector2f getVectorFromAngle(const double angle);
	const double getAngleDifference(const double angle1, const double angle2);

	template <class T>
	void printDebugMessage(T message) {
		std::ostringstream str;
		str << message;
		OutputDebugString(str.str().c_str());
	}
}
#endif