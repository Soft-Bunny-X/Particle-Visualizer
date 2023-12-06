#pragma once

#include <GL\glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();
	void RenderMeshParticle();
	void RenderMeshLineStrip();
	void RenderMeshLine();
	void UpdateMesh(GLfloat *vertices);

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
	GLsizei vertexCount;
};

