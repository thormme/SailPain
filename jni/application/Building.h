#ifndef BUILDING_H
#define BUILDING_H
#include <zenilib.h>
#include "GameObject.h"

class Building : public GameObject {
private:

public:
	Building(const Zeni::Point3f &position = Zeni::Point3f(),
		const Zeni::Quaternion &orientation = Zeni::Quaternion(),
		const Zeni::Model &model = Zeni::Model("models/plane.3ds"),
		const double scale = 10.0);

};
#endif