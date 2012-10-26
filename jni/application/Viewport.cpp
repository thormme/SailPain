#include <zenilib.h>
#include "Viewport.h"
#include "Sailplane.h"

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
	m_camera.position = m_trackedObject->getPosition() - facingDirection * 300.0f;
	double pitch = Utils::getAngleBetweenVectors(facingDirection, facingDirection.get_ij());
	if (facingDirection.k > 0.0) {
		pitch *= -1.0;
	}
	m_camera.orientation = Zeni::Quaternion(Utils::getAngleFromVector(Zeni::Vector2f(facingDirection.i, facingDirection.j)), pitch, 0.0f);
}
void Viewport::render(const Level &level, const std::vector<GameObject*> &objects) const {
	//Render World
	Zeni::Video &vr = Zeni::get_Video();
	Zeni::Vector2f screenSize = Zeni::Vector2f(Zeni::Point2f(vr.get_render_target_size()));
	vr.set_3d_view(m_camera,
		std::make_pair(Zeni::Vector2f(m_viewPosition).multiply_by(screenSize), Zeni::Vector2f(m_viewPosition + m_viewSize).multiply_by(screenSize)));

	Zeni::Light light = Zeni::Light(Zeni::Color(0.0, 0.3, 0.3, 0.3), Zeni::Color(0.0, 0.3, 0.3, 0.3));
	light.position = Zeni::Point3f(10, 10, 10);
	light.spot_direction = Zeni::Vector3f(1.0f, 1.0f, -4.0);
	light.set_light_type(Zeni::LIGHT_DIRECTIONAL);
	
	vr.set_ambient_lighting(Zeni::Color(0.0f, 0.0f, 0.0f, 0.0f));
	vr.set_Light(0, light);

	level.render();

	vr.set_lighting(true);

	for (int i=0; i < objects.size(); i++) {
		objects[i]->render();
	}

	vr.set_lighting(false);

	// Render HUD
	const Sailplane * plane = dynamic_cast<const Sailplane*>(m_trackedObject);

	if (plane != nullptr) {
		float healthX = 0.0f;
		float rocketX = healthX + 78.0f + 32.0f;
		float jumpX = rocketX + 78.0f + 32.0f;
		float mineX = jumpX + 78.0f + 32.0f;

		vr.set_2d_view(std::make_pair(Zeni::Vector2f(m_viewPosition).multiply_by(screenSize), Zeni::Vector2f(m_viewPosition + m_viewSize).multiply_by(screenSize)), std::make_pair(Zeni::Vector2f(m_viewPosition).multiply_by(screenSize), Zeni::Vector2f(m_viewPosition + m_viewSize).multiply_by(screenSize)));
		Zeni::Vector2f internalResolution = m_viewSize.multiply_by(screenSize);

		Zeni::Vector2f hudPosition = Zeni::Vector2f(internalResolution.i - 420.0, 0);
		Zeni::Video &vr = Zeni::get_Video();
		Zeni::Vector2f screenSize = Zeni::Vector2f(Zeni::Point2f(vr.get_render_target_size()));
		vr.set_2d_view(std::make_pair(Zeni::Point2f(0.0f, 0.0f), internalResolution),
			std::make_pair(Zeni::Vector2f(m_viewPosition).multiply_by(screenSize), Zeni::Vector2f(m_viewPosition + m_viewSize).multiply_by(screenSize)), true);
		Zeni::Vertex2f_Texture ul(Zeni::Point2f(healthX,  0.0f) + hudPosition, Zeni::Point2f(0.0f, 0.0f));
		Zeni::Vertex2f_Texture ll(Zeni::Point2f(healthX, 32.0f) + hudPosition, Zeni::Point2f(0.0f, 1.0f));
		Zeni::Vertex2f_Texture lr(Zeni::Point2f(healthX + 32.0f, 32.0f) + hudPosition, Zeni::Point2f(1.0f, 1.0f));
		Zeni::Vertex2f_Texture ur(Zeni::Point2f(healthX + 32.0f,  0.0f) + hudPosition, Zeni::Point2f(1.0f, 0.0f));
		Zeni::Material material("health");
		Zeni::Quadrilateral<Zeni::Vertex2f_Texture> quad(ul, ll, lr, ur);
		quad.fax_Material(&material);
		vr.render(quad);

		std::ostringstream str;
		str << plane->getHealth().remaining;
		Zeni::get_Fonts()["silkscreen_normal"].render_text(Zeni::String(str.str()), Zeni::Point2f(32, -6) + hudPosition, Zeni::Color(0xFFFFFFFF));
	}
}

const Zeni::Camera Viewport::getCamera() const {
	return m_camera;
}