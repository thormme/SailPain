#include <zenilib.h>
#include "Powerup.h"
#include "Utils.h"

Zeni::Model getModelFromType(Powerup::Type powerupType) {
	switch (powerupType) {
	case Powerup::Type::BOOST:
		return Zeni::Model("models/powerup_boost.3ds");
		break;
	case Powerup::Type::MISSILE:
		return Zeni::Model("models/powerup_missile.3ds");
		break;
	case Powerup::Type::HEALTH:
		return Zeni::Model("models/powerup_health.3ds");
		break;
	default:
		return Zeni::Model("models/powerup_boost.3ds");
	}
}

Powerup::Powerup(const Zeni::Point3f &position, Type powerupType) 
	: GameObject(position, Zeni::Quaternion(), getModelFromType(powerupType), CollisionGeometry("models/powerup.col"), Zeni::Vector3f(), Zeni::Vector3f(), 1.0, Zeni::Vector3f(10.0f, 10.0f, 10.0f)) {
	
	collideWithGameObjects();
	setYawRate(Utils::PI/10.0);
}