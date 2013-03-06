#ifndef _MODEL_H_
#define _MODEL_H_

#include <cstring>
#include <fstream>
#include "math_3d.h"

/* Vertex structure to store the point & the texture coordinate that it is mapped to */
struct Vertex
{
	Vector3f position;
	Vector2f texPos;
};

struct Triangle
{
	// this Triangle's vertices
	int v1, v2, v3;

	// the index of this triangle's normal & texture
	int normalIndex, textureIndex;

	// points to map the vertices to the texture image
	Vector2f textureCoord[3];
};

struct Texture
{
	// the height of this texture
	int width;
	// the width of this texture
	int height;

	// the binary format of this texture
	unsigned char* texture;
};

struct Model
{
	char* buildingName;

	int num_vertices;
	Vector3f* vertices;

	int num_triangles;
	Triangle* triangles;

	int num_normals;
	Vector3f* normals;

	int num_textures;
	Texture* textures;

	int numVertices()
	{
		return num_vertices;
	}

	int numTriangles()
	{
		return num_triangles;
	}

	int numNormals()
	{
		return num_normals;
	}

	int numTextures()
	{
		return num_textures;
	}

	char* getBuildingName()
	{
		return buildingName;
	}

	Vector3f* getNormals()
	{
		return normals;
	}

	void getVertices(Vertex* v);

	/* retrieve the vertex indices of the triangles to be drawn */
	void getTriangles(unsigned int* indices);

	Texture* getTexture(int index)
	{
		if(index < num_textures)
		{
			return &textures[index];
		}
		else
		{
			return NULL;
		}
	}

	~Model();
};

class ModelReader
{
private:
	void ReadBuildingName(std::ifstream& in, Model& m);
	void ReadVertices(std::ifstream& in, Model& m);
	void ReadTriangles(std::ifstream& in, Model& m);
	void ReadNormals(std::ifstream& in, Model& m);
	void ReadTextures(std::ifstream& in, Model& m);

public:
	ModelReader() {}

	void ReadModel(char* filename, Model& model);
};

#endif