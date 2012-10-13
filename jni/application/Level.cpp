#include <zenilib.h>
#include "Level.h"

double getHeightFromColor(Uint32 color) {
	return (double)(color & 0x000000FF)/256.0;
}

std::string getImageNameFromColor(Uint32 color) {
	return "dirt";
}

Level::Level(std::string fileName) {
	m_vertexSeparation = Zeni::Vector2f(32.0, 32.0);
	m_maxHeight = 128.0;

	Zeni::Image levelImage = Zeni::Image(Zeni::String("levels/" + fileName + "-heightmap.png"));
	// Create points
	for (int y=0; y < levelImage.height(); y++) {
		std::vector<HeightMapVertex> row;
		for (int x=0; x < levelImage.width(); x++) {
			Uint32 color = levelImage.extract_RGBA(Zeni::Point2i(x, y));
			std::string imageName = getImageNameFromColor(color);
			std::map<std::string, int>::iterator imageIndex = m_textureIndices.find(imageName);
			if (imageIndex == m_textureIndices.end()) {
				m_textureIndices.insert(std::make_pair(imageName, m_textureIndices.size()));
				imageIndex = m_textureIndices.find(imageName);
				m_trianglesByTexture.push_back(std::vector< Zeni::Triangle<Zeni::Vertex3f_Texture>* >());
			}
			Zeni::Vector3f position(x * m_vertexSeparation.i, y * m_vertexSeparation.j, getHeightFromColor(color) * m_maxHeight);
			HeightMapVertex newVertex(position, imageIndex->second);
			row.push_back(newVertex);
		}
		m_vertices.push_back(row);
	}
	// Parse points into triangles
	for (int x=0; x < m_vertices.size() - 1; x++) {
		for (int y=0; y < m_vertices[x].size() - 1; y++) {
			Zeni::Point3f normal = Zeni::Point3f((m_vertices[x][y].position - m_vertices[x+1][y].position)%(m_vertices[x][y].position - m_vertices[x][y+1].position));

			Zeni::Vertex3f_Texture ul(m_vertices[x][y].position, normal, Zeni::Point2f(0.0f, 0.0f));
			Zeni::Vertex3f_Texture ur(m_vertices[x+1][y].position, normal, Zeni::Point2f(1.0f, 0.0f));
			Zeni::Vertex3f_Texture bl(m_vertices[x][y+1].position, normal, Zeni::Point2f(0.0f, 1.0f));
			Zeni::Vertex3f_Texture br(m_vertices[x+1][y+1].position, normal, Zeni::Point2f(1.0f, 1.0f));

			Zeni::Triangle<Zeni::Vertex3f_Texture> * t1 = new Zeni::Triangle<Zeni::Vertex3f_Texture>(ul, bl, ur);
			Zeni::Triangle<Zeni::Vertex3f_Texture> * t2 = new Zeni::Triangle<Zeni::Vertex3f_Texture>(bl, br, ur);

			m_triangles.push_back(t1);
			m_triangles.push_back(t2);
			m_trianglesByTexture[m_vertices[x][y].textureIndex].push_back(t1);
			m_trianglesByTexture[m_vertices[x][y].textureIndex].push_back(t2);
		}
	}
}

void Level::render() const {
	for (int i=0; i < m_trianglesByTexture.size(); i++) {
		Zeni::get_Video().apply_Texture(Zeni::get_Textures()["dirt"]);
		for (int j=0; j < m_trianglesByTexture[i].size(); j++) {
			Zeni::get_Video().render(*m_trianglesByTexture[i][j]);
		}
		Zeni::get_Video().unapply_Texture();
	}
}