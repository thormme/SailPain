#include <zenilib.h>
#include "Sailplane.h"
#include "Utils.h"
#include "Input.h"

Sailplane::Sailplane(const Zeni::Point3f &position,
		const Zeni::Quaternion &orientation,
		const Zeni::Model &model,
		const Zeni::Vector3f &velocity,
		const Zeni::Vector3f &force,
		const double &mass) : GameObject(position, orientation, model, velocity, force, mass) {

}

void Sailplane::handleCollisions(const std::vector<GameObject*> &collisions) {
	for (int i=0; i < collisions.size(); i++) {
		Utils::printDebugMessage("hi!\n");
	}
}

const StateModifications Sailplane::act(const std::vector<GameObject*> &collisions) {
	setYawRate(0.0);
	setPitchRate(0.0);
	setRollRate(0.0);
	setForce(Zeni::Vector3f());
	if (Input::isKeyDown(SDLK_LEFT)) {
		setYawRate(Utils::PI/2.0);
	}
	if (Input::isKeyDown(SDLK_RIGHT)) {
		setYawRate(-Utils::PI/2.0);
	}
	if (Input::isKeyDown(SDLK_UP)) {
		setForce(getForwardVector()*10);
	}
	if (Input::isKeyDown(SDLK_DOWN)) {
		setForce(getForwardVector()*10);
	}
	return StateModifications();
}