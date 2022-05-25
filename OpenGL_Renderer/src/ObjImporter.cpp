#include "pch.h"
#include "ObjImporter.h"

ObjImporter::ObjImporter(const std::string& filename, bool genTangentCoord)
	:m_Vertecies(), m_Indecies(), m_GenTangentCoord(genTangentCoord)
{
	std::ifstream file;
	file.open(filename);
	if (file.fail())
	{
		std::cout << "[ObjImporter]::File failed to open!" << std::endl;
		return;
	}
	std::string line;
	std::vector<Vec3f> positions;
	std::vector<Vec2f> texCoords;
	std::vector<Vec3i> texCoordsIndex;
	std::vector<Vec3f> normals;
	std::vector<Vec3i> normalIndex;
	Vec3f colorr[3] = {{ 1.0f ,0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }};

	Vec3f tangent, bitangent;

	while (!file.eof()) {
		std::getline(file, line);
		std::istringstream sstr(line.c_str());
		char trash;
		int trash1;
		if (!line.compare(0, 2, "v ")) {
			Vec3f v = { 0.0f, 0.0f, 0.0f };
			sstr >> trash >> v.x >> v.y >> v.z;
			positions.push_back(v);
		}
		else if (!line.compare(0, 2, "f ")) {
			Vec3i face;
			Vec3i texID;
			Vec3i normID;
			sstr >> trash >> face.x >> trash >> texID.x >> trash >> normID.x >>
						     face.y >> trash >> texID.y >> trash >> normID.y >>
							 face.z >> trash >> texID.z >> trash >> normID.z;

			face.x--; face.y--; face.z--;
			texID.x--; texID.y--; texID.z--;
			normID.x--; normID.y--; normID.z--;

			texCoordsIndex.push_back(texID);
			m_Indecies.push_back(face);
			normalIndex.push_back(normID);
		}
		else if (!line.compare(0, 2, "vt")) {
			Vec2f vt;
			sstr >> trash >> trash >> vt.x >> vt.y;
			texCoords.push_back(vt);
		}
		else if (!line.compare(0, 2, "vn")) {
			Vec3f vn;
			sstr >> trash >> trash >> vn.x >> vn.y >> vn.z;
			normals.push_back(vn);
		}
	}
	for (unsigned int i = 0; i < m_Indecies.size(); i++)
	{
		if (m_GenTangentCoord) {

			Vec3f edge1 = positions[m_Indecies[i].y] - positions[m_Indecies[i].x];
			Vec3f edge2 = positions[m_Indecies[i].z] - positions[m_Indecies[i].x];
			Vec2f deltaUV1 = texCoords[texCoordsIndex[i].y] - texCoords[texCoordsIndex[i].x];
			Vec2f deltaUV2 = texCoords[texCoordsIndex[i].z] - texCoords[texCoordsIndex[i].x];

			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

			bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

			m_VerteciesTBN.push_back({
				{ positions[m_Indecies[i].x] },
				{ colorr[rand() % 3] },
				{ texCoords[texCoordsIndex[i].x] },
				{ normals[normalIndex[i].x] }, 
				{ tangent.x, tangent.y, tangent.z},
				{ bitangent.x, bitangent.y, bitangent.z} });

			m_VerteciesTBN.push_back({
				{ positions[m_Indecies[i].y] },
				{ colorr[rand() % 3] },
				{ texCoords[texCoordsIndex[i].y] },
				{ normals[normalIndex[i].y] },
				{ tangent.x, tangent.y, tangent.z},
				{ bitangent.x, bitangent.y, bitangent.z} });

			m_VerteciesTBN.push_back({
				{ positions[m_Indecies[i].z] },
				{ colorr[rand() % 3] },
				{ texCoords[texCoordsIndex[i].z] },
				{ normals[normalIndex[i].z] },
				{ tangent.x, tangent.y, tangent.z},
				{ bitangent.x, bitangent.y, bitangent.z} });
		}
		else {
			m_Vertecies.push_back({
				{ positions[m_Indecies[i].x] },
				//{ colorr[rand() % 3] },
				{ texCoords[texCoordsIndex[i].x] },
				{ normals[normalIndex[i].x] } });

			m_Vertecies.push_back({
				{ positions[m_Indecies[i].y] },
				//{ colorr[rand() % 3] },
				{ texCoords[texCoordsIndex[i].y] },
				{ normals[normalIndex[i].y] } });

			m_Vertecies.push_back({
				{ positions[m_Indecies[i].z] },
				//{ colorr[rand() % 3] },
				{ texCoords[texCoordsIndex[i].z] },
				{ normals[normalIndex[i].z] } });
		}

	}
}

ObjImporter::~ObjImporter()
{
}

void* ObjImporter::GetVertecies() 
{
	if (m_GenTangentCoord) {
		void* vecPointer = m_VerteciesTBN.data();
		return vecPointer;
	}
	else {
		void* vecPointer = m_Vertecies.data();
		return vecPointer;
	}

}

unsigned int ObjImporter::GetVertCount()
{
	if (m_GenTangentCoord) {
		return m_VerteciesTBN.size() * sizeof(struct VertexTBN);
	} else 
		return m_Vertecies.size() * sizeof(struct Vertex);
}


unsigned int* ObjImporter::GetIndecies()
{
	void* vecPointer = m_Indecies.data();
	return (unsigned int*)vecPointer;
}

unsigned int ObjImporter::GetIndeciesCount()
{
	return m_Indecies.size()*3;
}
