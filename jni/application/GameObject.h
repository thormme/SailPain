#include <zenilib.h>

class GameObject {
private:
	Zeni::Point3f m_position;

public:
	GameObject(Zeni::Point3f position = Zeni::Point3f());

	void stepPhysics(const double timeStep);
	void handleCollisions();
	void act();

	void render() const;

	const Zeni::Point3f getPosition() const;
	void setPosition(Zeni::Point3f position);

};