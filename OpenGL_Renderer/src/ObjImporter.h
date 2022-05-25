#pragma once

 struct Vec3f {
	float x;
	float y;
	float z;
 public:
	 friend Vec3f operator-(Vec3f p0, Vec3f p1)
	{
		Vec3f result;

		result.x = p0.x - p1.x;
		result.y = p0.y - p1.y;
		result.z = p0.z - p1.z;

		return result;
	}
};

struct Vec3i {
	unsigned int x;
	unsigned int y;
	unsigned int z;
};

struct Vec2f {
	float x;
	float y;
	friend Vec2f operator-(Vec2f p0, Vec2f p1)
	{
		Vec2f result;

		result.x = p0.x - p1.x;
		result.y = p0.y - p1.y;

		return result;
	}
};

struct Vertex {
	Vec3f pos;
	//Vec3f color;
	Vec2f texCoord;
	Vec3f normal;
};

struct VertexTBN {
	Vec3f pos;
	Vec3f color;
	Vec2f texCoord;
	Vec3f normal;
	Vec3f tangent;
	Vec3f bitangent;
};

class ObjImporter
{
private:
	std::vector<Vertex> m_Vertecies;
	std::vector<VertexTBN> m_VerteciesTBN;
	std::vector<Vec3i> m_Indecies;
	bool m_GenTangentCoord;
public:
	ObjImporter(const std::string& filename, bool genTangentCoord);
	~ObjImporter();

	void* GetVertecies();
	unsigned int GetVertCount();

	unsigned int* GetIndecies();
	unsigned int GetIndeciesCount();
};

