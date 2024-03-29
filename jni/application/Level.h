#ifndef LEVEL_H
#define LEVEL_H
#include <zenilib.h>

class Level {
private:
	struct HeightMapVertex {
		Zeni::Point3f position;
		int textureIndex;

		HeightMapVertex(Zeni::Point3f position, int textureIndex) : position(position), textureIndex(textureIndex) {}
	};

	Zeni::Vector2f m_vertexSeparation;
	double m_maxHeight;
	std::vector< std::vector<HeightMapVertex> > m_vertices;

	/*struct HeightMapVertex {
		Zeni::Point3f position;
		std::string texture;
		std::vector< std::vector<Zeni::Triangle<Zeni::Vertex2f_Texture>* > > triangles;

		HeightMapVertex(Zeni::Point3f position, std::string texture, std::vector< std::vector<Zeni::Triangle<Zeni::Vertex2f_Texture>* > > triangles;) 
			: position(position), texture(texture), triangles(triangles) {}
	};*/

	std::map<std::string, int> m_textureIndices;
	std::vector< std::vector< Zeni::Triangle<Zeni::Vertex3f_Texture>* > > m_trianglesByTexture;
	std::vector< Zeni::Triangle<Zeni::Vertex3f_Texture>* > m_triangles;
	//std::vector< std::vector<Zeni::Triangle<Zeni::Vertex3f_Texture>* > > m_vertexTriangles;

public:
	Level(std::string fileName = "level1", Zeni::Vector3f levelSize = Zeni::Vector3f(64.0f, 64.0f, 128.0f));
	~Level();

	void render() const;

	const Zeni::Point3f Level::getPositionAtPoint(Zeni::Point2f point) const;
	const Zeni::Collision::Plane Level::getPlaneAtPoint(Zeni::Point2f point) const;
	const Zeni::Vector3f getBounds() const;

};
#endif