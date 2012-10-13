#include <zenilib.h>
#include "GameObject.h"

GameObject::GameObject(Zeni::Point3f position) : m_position(position) {

}

void GameObject::stepPhysics(const double timeStep) {

}

void GameObject::handleCollisions() {

}

void GameObject::act() {

}

void GameObject::render() const {
	Zeni::Vertex3f_Texture ul(getPosition() + Zeni::Point3f(-10.0f, 0.0f, 10.0f), Zeni::Point2f(0.0f, 0.0f));
	Zeni::Vertex3f_Texture ur(getPosition() + Zeni::Point3f(10.0f, 0.0f, 10.0f), Zeni::Point2f(1.0f, 0.0f));
	Zeni::Vertex3f_Texture bl(getPosition() + Zeni::Point3f(-10.0f, 0.0f, -10.0f), Zeni::Point2f(0.0f, 1.0f));
	Zeni::Vertex3f_Texture br(getPosition() + Zeni::Point3f(10.0f, 0.0f, -10.0f), Zeni::Point2f(1.0f, 1.0f));

	Zeni::Quadrilateral<Zeni::Vertex3f_Texture> quad(ul, bl, br, ur);
	quad.lend_Material(&Zeni::Material("placeholder"));

	Zeni::get_Video().render(quad);
}

const Zeni::Point3f GameObject::getPosition() const {
	return m_position;
}

void GameObject::setPosition(Zeni::Point3f position) {
	m_position = position;
}
