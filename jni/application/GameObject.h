#include <zenilib.h>

class GameObject;

struct StateModifications {
	std::list<GameObject*> gameObjectAdditions;
	std::list<GameObject*> gameObjectRemovals;

	void combine(const StateModifications &stateModifications) {
		gameObjectAdditions.insert(gameObjectAdditions.end(), stateModifications.gameObjectAdditions.begin(), stateModifications.gameObjectAdditions.end());
		gameObjectRemovals.insert(gameObjectRemovals.end(), stateModifications.gameObjectRemovals.begin(), stateModifications.gameObjectRemovals.end());
	}
};

class GameObject {
private:
	Zeni::Model m_model;

	Zeni::Point3f m_position;
	Zeni::Vector3f m_velocity;
	Zeni::Vector3f m_force;
	double m_mass;
	double m_yawRate;
	double m_pitchRate;
	double m_rollRate;
	Zeni::Quaternion m_orientation;
	bool m_collideWithGameObjects;

public:
	GameObject(const Zeni::Point3f &position = Zeni::Point3f(),
		const Zeni::Quaternion &orientation = Zeni::Quaternion(),
		const Zeni::Model &model = Zeni::Model("models/placeholder.3ds"),
		const Zeni::Vector3f &velocity = Zeni::Vector3f(),
		const Zeni::Vector3f &force = Zeni::Vector3f(),
		const double &mass = 1.0);

	const bool isTouching(GameObject* object) const;

	void stepPhysics(const double timeStep);
	void handleCollisions(const std::vector<GameObject*> &collisions);
	const StateModifications act(const std::vector<GameObject*> &collisions);

	void render();

	const Zeni::Point3f getPosition() const;
	void setPosition(Zeni::Point3f position);
	const Zeni::Quaternion getOrientation() const;
	void setOrientation(Zeni::Quaternion orientation);
	const bool willDetectCollisionsWithGameObjects();
};