#ifndef VIEWPORT_H
#define VIEWPORT_H
#include <zenilib.h>
#include "Level.h"
#include "GameObject.h"
#include "Utils.h"

class Viewport {

protected:
	Zeni::Point2f m_viewPosition;
	Zeni::Vector2f m_viewSize;
	Zeni::Camera m_camera;

	const GameObject * m_trackedObject;

public:
	Viewport(const GameObject * trackedObject,
			 const Zeni::Point2f &viewPosition = Zeni::Point2f(), 
			 const Zeni::Vector2f &viewSize = Zeni::Vector2f(1.0f, 1.0f), 
			 const Zeni::Camera &camera = Zeni::Camera(Zeni::Point3f(0.0f, 0.0f, 0.0f), Zeni::Quaternion(), 10.0f, 4000.0f, Utils::PI*2.25f, 1.0f));

	virtual ~Viewport();

	void stepViewportPosition(double timeStep);
	virtual void render(const Level &level, const std::vector<GameObject*> &objects) const;

	const Zeni::Camera getCamera() const;

};
#endif