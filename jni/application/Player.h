#ifndef PLAYER_H
#define PLAYER_H
#include <zenilib.h>
#include "Sailplane.h"
#include "GameObject.h"
#include "Input.h"

class Sailplane;

class Player {
	
public:
	Player();

	Sailplane * getNewPlane(const Zeni::Point3f &position = Zeni::Point3f(0.0f, 0.0f, 0.0f), const Zeni::Quaternion orientation = Zeni::Quaternion());

	Sailplane * getLastPlane();

	void setControls(const std::vector<InputAction> &controls);
	void setControls(int joyIndex = -1); ///< Set to default control scheme

	void setPlaneModel(const Zeni::String &model);
	const Zeni::String getPlaneModel() const;

	virtual const StateModifications drivePlane(Sailplane &sailplane, const std::vector<GameObject*> &gameObjectCollisions);

	virtual void senseSurroundings(std::vector<GameObject*> objects) {}

private:
	enum Controls {FORWARD = 0, BACKWARD, LEFT, RIGHT, FIRE, SPECIAL, FINAL_ELEMENT};
	// Player attributes
	std::vector<InputAction> m_controls;

	Sailplane* m_plane;
	// Attributes to pass to car
	Zeni::String m_planeModel;
protected:

};
#endif