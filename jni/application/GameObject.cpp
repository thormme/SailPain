#include <zenilib.h>
#include "GameObject.h"
#include "Input.h"
#include "Utils.h"
#include "lib3ds.h"

GameObject::GameObject(const Zeni::Point3f &position,
		const Zeni::Quaternion &orientation,
		const Zeni::Model &model,
		const CollisionGeometry &collisionGeometry,
		const Zeni::Vector3f &velocity,
		const Zeni::Vector3f &force,
		const double mass,
		const double scale)
		: m_position(position),
		m_orientation(orientation),
		m_model(model),
		m_collisionGeometry(collisionGeometry),
		m_velocity(velocity),
		m_force(force),
		m_mass(mass),
		m_scale(Zeni::Vector3f(scale, scale, scale)) {
	m_collideWithGameObjects = false;
	m_yawRate = 0.0;
	m_pitchRate = 0.0;
	m_rollRate = 0.0;
	m_model.set_scale(m_scale);
}

const bool doFacesIntersect(const Zeni::Point3f face1[3], const Zeni::Point3f face2[3]) {
	for (int i=0; i < 3; i++) {
		Zeni::Collision::Line_Segment lineSegment(face1[i], face1[(i+1)%3]);
		Zeni::Point3f intersectionPoint = Utils::getFaceLineSegmentIntersection(lineSegment, face2);
		if (Utils::isPointWithinFace(intersectionPoint, face2)) {
			return true;
		}
	}
	for (int i=0; i < 3; i++) {
		Zeni::Collision::Line_Segment lineSegment(face2[i], face2[(i+1)%3]);
		Zeni::Point3f intersectionPoint = Utils::getFaceLineSegmentIntersection(lineSegment, face1);
		if (Utils::isPointWithinFace(intersectionPoint, face1)) {
			return true;
		}
	}
	return false;
}

const bool GameObject::isTouching(GameObject* object) {
	double boundingRadius = (m_model.get_extents().lower_bound - m_model.get_extents().upper_bound).magnitude() / 2.0;
	double objectBoundingRadius = (object->m_model.get_extents().lower_bound - object->m_model.get_extents().upper_bound).magnitude() / 2.0;
	double minimumSeparation = boundingRadius + objectBoundingRadius;
	if ((getPosition() - object->getPosition()).magnitude() > minimumSeparation) {
		return false;
	}

	/*Lib3dsFile * file = m_model.get_file();
	Lib3dsFile * objectFile = object->m_model.get_file();

	// Get vertices for object model
	std::vector< std::vector<Zeni::Point3f> > objectMeshes;
	objectMeshes.reserve(objectFile->nmeshes);
	for (int i=0; i < objectFile->nmeshes; i++) {
		std::vector<Zeni::Point3f> vertices;
		vertices.reserve(objectFile->meshes[i]->nvertices);

		for (int j=0; j < objectFile->meshes[i]->nvertices; j++) {
			Zeni::Vector3f vertex(objectFile->meshes[i]->vertices[j][0], objectFile->meshes[i]->vertices[j][1], objectFile->meshes[i]->vertices[j][2]);
			vertices.push_back((Zeni::Quaternion::Axis_Angle(object->m_model.get_rotate().first, object->m_model.get_rotate().second) * (vertex.multiply_by(object->m_model.get_scale()))) + Zeni::Vector3f(object->m_model.get_translate()));
		}

		objectMeshes.push_back(vertices);
	}

	for (int i=0; i < file->nmeshes; i++) {
		std::vector<Zeni::Point3f> vertices;
		vertices.reserve(file->meshes[i]->nvertices);

		for (int j=0; j < file->meshes[i]->nvertices; j++) {
			Zeni::Vector3f vertex(file->meshes[i]->vertices[j][0], file->meshes[i]->vertices[j][1], file->meshes[i]->vertices[j][2]);
			vertices.push_back((Zeni::Quaternion::Axis_Angle(m_model.get_rotate().first, m_model.get_rotate().second) * (vertex.multiply_by(m_model.get_scale()))) + Zeni::Vector3f(m_model.get_translate()));
		}

		for (int j=0; j < file->meshes[i]->nfaces; j++) {
			Zeni::Point3f face1[] = { vertices[file->meshes[i]->faces[j].index[0]],	vertices[file->meshes[i]->faces[j].index[1]], vertices[file->meshes[i]->faces[j].index[2]] };
			for (int k = 0; k < objectFile->nmeshes; k++) {
				for (int l=0; l < objectFile->meshes[i]->nfaces; l++) {
					Zeni::Point3f face2[] = { objectMeshes[i][file->meshes[i]->faces[j].index[0]],
											  objectMeshes[i][file->meshes[i]->faces[j].index[1]],
											  objectMeshes[i][file->meshes[i]->faces[j].index[2]] };
					if (doFacesIntersect(face1, face2)) {
						return true;
					}
				}
			}
		}
	}
	return false;*/
	return m_collisionGeometry.isTouching(object->m_collisionGeometry, m_position, object->m_position, m_orientation, object->m_orientation, m_scale, object->m_scale);
}

void GameObject::stepPhysics(const double timeStep) {
	setOrientation(getOrientation() * Zeni::Quaternion(m_yawRate * timeStep, m_pitchRate * timeStep, m_rollRate * timeStep));
	Zeni::Vector3f acceleration = m_force / m_mass;
	m_velocity += acceleration * timeStep;
	setPosition(getPosition() + m_velocity * timeStep);
}

void GameObject::handleCollisions(const std::vector<GameObject*> &collisions) {
	
}

const StateModifications GameObject::act(const std::vector<GameObject*> &collisions) {
	return StateModifications();
}

void GameObject::render() {
	m_model.render();
}

const Zeni::Point3f GameObject::getPosition() const {
	return m_position;
}

void GameObject::setPosition(Zeni::Point3f position) {
	m_position = position;
	m_model.set_translate(m_position);
}

const Zeni::Vector3f GameObject::getVelocity() const {
	return m_velocity;
}

void GameObject::setVelocity(Zeni::Vector3f velocity) {
	m_velocity = velocity;
}

const Zeni::Vector3f GameObject::getForce() const {
	return m_force;
}

void GameObject::setForce(Zeni::Vector3f force) {
	m_force = force;
}

const Zeni::Quaternion GameObject::getOrientation() const {
	return m_orientation;
}

const Zeni::Vector3f GameObject::getForwardVector() const {
	return (m_orientation*Zeni::Vector3f(1.0f, 0.0f, 0.0f)).normalize();
}

void GameObject::setOrientation(Zeni::Quaternion orientation) {
	m_orientation = orientation;
	m_model.set_rotate(m_orientation);
}

const double GameObject::getYawRate() const {
	return m_yawRate;
}

void GameObject::setYawRate(double rate) {
	m_yawRate = rate;
}

const double GameObject::getPitchRate() const {
	return m_pitchRate;
}

void GameObject::setPitchRate(double rate) {
	m_pitchRate = rate;
}

const double GameObject::getRollRate() const {
	return m_rollRate;
}

void GameObject::setRollRate(double rate) {
	m_rollRate = rate;
}

const double GameObject::getMass() const {
	return m_mass;
}

void GameObject::collideWithGameObjects(bool collide) {
	m_collideWithGameObjects = collide;
}

const bool GameObject::willCollideWithGameObjects() const {
	return m_collideWithGameObjects;
}

void GameObject::detectCollisionsWithGameObjects(bool detect) {
	m_detectCollisionsWithGameObjects = detect;
}

const bool GameObject::willDetectCollisionsWithGameObjects() const {
	return m_detectCollisionsWithGameObjects;
}