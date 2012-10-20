#include <zenilib.h>
#include "Viewport.h"

Viewport::Viewport(const GameObject * trackedObject,
			 const Zeni::Point2f &viewPosition, 
			 const Zeni::Vector2f &viewSize, 
			 const Zeni::Camera &camera) {
	m_viewPosition = viewPosition;
	m_viewSize = viewSize;
	m_camera = camera;
	m_trackedObject = trackedObject;
}
Viewport::~Viewport(){}

void Viewport::stepViewportPosition(double timeStep) {
	Zeni::Vector3f facingDirection = m_trackedObject->getVelocity().normalized();
	m_camera.position = m_trackedObject->getPosition() - facingDirection * 200.0f;
	double pitch = facingDirection.angle_between(facingDirection.get_ij());
	if (facingDirection.k > 0.0) {
		pitch *= -1.0;
	}
	m_camera.orientation = Zeni::Quaternion(Utils::getAngleFromVector(Zeni::Vector2f(facingDirection.i, facingDirection.j)), pitch, 0.0f);
}
void Viewport::render(const Level &level, const std::vector<GameObject*> &objects) const {
	Zeni::Video &vr = Zeni::get_Video();
	Zeni::Vector2f screenSize = Zeni::Vector2f(Zeni::Point2f(vr.get_render_target_size()));
	vr.set_3d_view(m_camera,
		std::make_pair(Zeni::Vector2f(m_viewPosition).multiply_by(screenSize), Zeni::Vector2f(m_viewPosition + m_viewSize).multiply_by(screenSize)));

	Zeni::Light light = Zeni::Light(Zeni::Color(0.0, 0.3, 0.3, 0.3), Zeni::Color(0.0, 0.3, 0.3, 0.3));
	light.position = Zeni::Point3f(10, 10, 10);
	light.set_spot_phi(Utils::PI/6.0);
	light.set_light_type(Zeni::LIGHT_DIRECTIONAL);
	vr.set_lighting(true);
	vr.set_ambient_lighting(Zeni::Color(0.0f, 0.0f, 0.0f, 0.0f));
	vr.set_Light(0, light);

	level.render();

	for (int i=0; i < objects.size(); i++) {
		objects[i]->render();
	}

	vr.set_lighting(false);
}

const Zeni::Camera Viewport::getCamera() const {
	return m_camera;
}