#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"

using namespace std;

struct Vertex_Material
{
	// Position
	glm::vec3 Position;
	// Normal
	glm::vec3 Normal;
};

struct Material {
	//color difuso
    aiColor3D diffuse;
	//color especular
    aiColor3D specular;
	//color ambiental
    aiColor3D ambient;
	//shininess
	float shininess;
};

class Mesh_Material
{
public:
	/*  Mesh Data  */
	vector<Vertex_Material> vertices;
	vector<GLuint> indices;
    Material material;

	/*  Functions  */
	// Constructor
	Mesh_Material(vector<Vertex_Material> vertices, vector<GLuint> indices,Material material)
	{
		this->vertices = vertices;
		this->indices = indices;
        this->material = material;

		// Now that we have all the required data, set the vertex buffers and its attribute pointers.
		this->setupMesh();
	}

	// Render the mesh
	void Draw(Shader shader)
	{
		// Bind appropriate textures
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		//if (this->textures.size()==0) {
		//	std::cout << "No hay texturas para este objeto" << std::endl;
		//}
		//for (GLuint i = 0; i < this->textures.size(); i++)
		//{
		//	glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
		//									  // Retrieve texture number (the N in diffuse_textureN)
		//	stringstream ss;
		//	string number;
		//	string name = this->textures[i].type;

		//	if (name == "texture_diffuse")
		//	{
		//		ss << diffuseNr++; // Transfer GLuint to stream
		//	}
		//	else if (name == "texture_specular")
		//	{
		//		ss << specularNr++; // Transfer GLuint to stream
		//	}

		//	number = ss.str();
		//	// Now set the sampler to the correct texture unit
		//	glUniform1i(glGetUniformLocation(shader.Program, (name + number).c_str()), i);
		//	// And finally bind the texture
		//	glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
		//}

		// Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
		//glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 16.0f);
        /*
        cout << material.diffuse.r << "\t";
        cout << material.diffuse.g << "\t";
        cout << material.diffuse.b << endl;
        cout << material.shininess << endl;
        */

		/*
        glUniform3f(glGetUniformLocation(shader.Program, "material.ambient"), material.ambient.r, material.ambient.g, material.ambient.b);
        glUniform3f(glGetUniformLocation(shader.Program, "material.diffuse"), material.diffuse.r, material.diffuse.g, material.diffuse.b);
        glUniform3f(glGetUniformLocation(shader.Program, "material.specular"), material.specular.r, material.specular.g, material.specular.b);
        glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), material.shininess);
		*/
		//puts("Dibujando con el material shader");
		// Draw mesh
		glBindVertexArray(this->VAO);
		//printf("Numero de triganulos  %i", this->indices.size());
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

private:
	/*  Render data  */
	GLuint VAO, VBO, EBO;

	/*  Functions    */
	// Initializes all the buffer objects/arrays
	void setupMesh()
	{
		// Create buffers/arrays
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);
		// Load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		// A great thing about structs is that their memory layout is sequential for all its items.
		// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		// again translates to 3/2 floats which translates to a byte array.
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex_Material), &this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		// Set the vertex attribute pointers
		// Vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Material), (GLvoid *)0);
		// Vertex Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_Material), (GLvoid *)offsetof(Vertex_Material, Normal));
		

		glBindVertexArray(0);
	}
};