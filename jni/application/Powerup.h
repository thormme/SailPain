#ifndef POWERUP_H
#define POWERUP_H
#include <zenilib.h>
#include "GameObject.h"

class Powerup : public GameObject {
public:
	enum Type {BOOST = 0, MISSILE, HEALTH, FINAL_ELEMENT};

	Powerup(const Zeni::Point3f &position = Zeni::Point3f(), Type powerupType = Type::BOOST);

	const Type getType() const;

private:
	Type type;

};
#endif