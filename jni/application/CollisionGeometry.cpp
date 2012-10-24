#include <zenilib.h>
#include <sstream>
#include "CollisionGeometry.h"

CollisionGeometry::CollisionGeometry(std::string fileName) {
	Zeni::String fileData;
	Zeni::File_Ops::load_asset(fileData, Zeni::String(fileName));
	std::string s = "";
	for (int i=0; i<fileData.size(); i++) {
		if (fileData[i] != '\r') {
			s += fileData[i];
		}
	}
	std::istringstream fileDataStream(s);
	std::string line;
	for (;std::getline(fileDataStream, line);) {
		std::istringstream lineStream(line);
		std::string objectType;
		lineStream >> objectType;
		if (objectType == "Parallelepiped") {
			Zeni::Vector3f pVecs[4];
			for (int i = 0; i < 4; i++) {
				std::getline(fileDataStream, line);
				std::istringstream nextLineStream(line);
				float x, y, z;
				nextLineStream >> x >> y >> z;
				pVecs[i] = Zeni::Vector3f(x, y, z);
			}
			m_geometry.push_back(Zeni::Collision::Parallelepiped(pVecs[0], pVecs[1], pVecs[2], pVecs[3]));
		}
	}
}

const bool CollisionGeometry::isTouching(const CollisionGeometry &collisionGeometry, 
		const Zeni::Point3f &position1,
		const Zeni::Point3f &position2,
		const Zeni::Quaternion &orientation1,
		const Zeni::Quaternion &orientation2,
		const Zeni::Vector3f &scale1,
		const Zeni::Vector3f &scale2) const {
	std::vector<Zeni::Collision::Parallelepiped> geometry;
	for (int i=0; i < m_geometry.size(); i++) {
		Zeni::Point3f position = orientation1 * Zeni::Vector3f(m_geometry[i].get_point()).multiply_by(scale1) + Zeni::Vector3f(position1);
		Zeni::Point3f sideA = orientation1 * m_geometry[i].get_edge_a().multiply_by(scale1);
		Zeni::Point3f sideB = orientation1 * m_geometry[i].get_edge_b().multiply_by(scale1);
		Zeni::Point3f sideC = orientation1 * m_geometry[i].get_edge_c().multiply_by(scale1);
		geometry.push_back(Zeni::Collision::Parallelepiped(position, sideA, sideB, sideC));
	}
	for (int i=0; i < collisionGeometry.m_geometry.size(); i++) {
		Zeni::Point3f position = orientation2 * Zeni::Vector3f(collisionGeometry.m_geometry[i].get_point()).multiply_by(scale2) + Zeni::Vector3f(position2);
		Zeni::Point3f sideA = orientation2 * collisionGeometry.m_geometry[i].get_edge_a().multiply_by(scale2);
		Zeni::Point3f sideB = orientation2 * collisionGeometry.m_geometry[i].get_edge_b().multiply_by(scale2);
		Zeni::Point3f sideC = orientation2 * collisionGeometry.m_geometry[i].get_edge_c().multiply_by(scale2);
		Zeni::Collision::Parallelepiped otherGeometry(position, sideA, sideB, sideC);
		for (int j=0; j < geometry.size(); j++) {
			if (geometry[j].intersects(otherGeometry)) {
				return true;
			}
		}
	}
	return false;
}