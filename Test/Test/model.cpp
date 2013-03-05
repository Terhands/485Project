#include "model.h"
#include "PPM.hpp"

#include <iostream>  // I/O 
#include <fstream>   // file I/O
#include <iomanip>   // format manipulation
#include <string>

void ModelReader::ReadModel(char* filename, Model& model)
{
	std::ifstream fin;

	fin.open(filename, std::ios::in);

	if(fin.good())
	{
		ReadBuildingName(fin, model);
		ReadVertices(fin, model);
		ReadTriangles(fin, model);
		ReadNormals(fin, model);
		ReadTextures(fin ,model);
	}

	fin.close();
}

void ModelReader::ReadBuildingName(std::ifstream& in, Model& m)
{
	int lineLength = 0;
	char line[200];
	in.getline(line, 200);
	while(line[lineLength] != '\n' && lineLength < 200)
	{
		lineLength += 1;
	}

	m.buildingName = new char[lineLength];
	strcpy(m.buildingName, line);
}

void ModelReader::ReadVertices(std::ifstream& in, Model& m)
{
	in >> m.num_vertices;

	m.vertices = new Vector3f[m.num_vertices];

	for(int i = 0; i < m.num_vertices; i++)
	{
		in >> m.vertices[i].x;
		in >> m.vertices[i].y;
		in >> m.vertices[i].z;
	}
}

void ModelReader::ReadTriangles(std::ifstream& in, Model& m)
{
	in >> m.num_triangles;

	m.triangles = new Triangle[m.num_triangles];

	for(int i = 0; i < m.num_triangles; i++)
	{
		in >> m.triangles[i].v1;
		in >> m.triangles[i].v2;
		in >> m.triangles[i].v3;

		in >> m.triangles[i].normalIndex;
		in >> m.triangles[i].textureIndex;

		in >> m.triangles[i].v1tx;
		in >> m.triangles[i].v1ty;
		in >> m.triangles[i].v2tx;
		in >> m.triangles[i].v2ty;
		in >> m.triangles[i].v3tx;
		in >> m.triangles[i].v3ty;
	}
}

void ModelReader::ReadNormals(std::ifstream& in, Model& m)
{
	in >> m.num_normals;

	m.normals = new Vector3f[m.num_normals];

	for(int i = 0; i < m.num_normals; i++)
	{
		in >> m.normals[i].x;
		in >> m.normals[i].y;
		in >> m.normals[i].z;
	}
}

void ModelReader::ReadTextures(std::ifstream& in, Model& m)
{
	in >> m.num_textures;

	m.textures = new Texture[m.num_textures];

	std::string currentTexture;
	for(int i = 0; i< m.num_textures; i++)
	{
		in >> currentTexture;
		m.textures[i].texture = PPM::Read(currentTexture, m.textures[i].width, m.textures[i].height);
	}
}

void Model::getTriangles(unsigned int* indices)
{
	for(int i = 0, j = 0; j < num_triangles; i += 3, j += 1)
	{
		indices[i] = triangles[j].v1;
		indices[i + 1] = triangles[j].v2;
		indices[i + 2] = triangles[j].v3;
	}
}

// routine to free up all of the dynamically allocated portions of a model
Model::~Model()
{
	if(NULL != buildingName) { delete [] buildingName; buildingName = NULL; }
	if(NULL != vertices) { delete [] vertices; vertices = NULL; }
	if(NULL != triangles) { delete [] triangles; triangles = NULL; }
	if(NULL != normals) { delete [] normals; normals = NULL; }

	for (int i = 0; i < num_textures; i++)
	{
		if (textures[i].texture != NULL)
		{
			delete [] textures[i].texture;
			textures[i].texture = NULL;
		}
	}

	if(NULL != textures) { delete [] textures; textures = NULL; }
}