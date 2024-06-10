#ifndef UMESH_H
#define UMESH_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include <GLFW/glfw3.h>
#include <string>

struct uFace {
public:
	unsigned int verts[3];
	bool show = true;
	uFace();
	uFace(unsigned int indexA, unsigned int indexB, unsigned int indexC);
};

/**
* Here we have a class simplifying the use of VBOs.
* IMPORTANT:
* Built in vertex attributes for shaders are as follows:
* gl_Vertex- 0
* gl_Normal- 2
* gl_Color- 3
* gl_SecondaryColor- 4
* gl_FogCoord- 5
* gl_MultiTexCoord0- 8
* gl_MultiTexCoord1- 9
* gl_MultiTexCoord2- 10
* gl_MultiTexCoord3- 11
* gl_MultiTexCoord4- 12
* gl_MultiTexCoord5- 13
* gl_MultiTexCoord6- 14
* gl_MultiTexCoord7- 15
* @author Adam
*/

class uMesh {
public:
	std::vector<glm::vec3> stream[16];
	std::vector<uFace> faces;

	GLuint indexBufferID;
	GLuint bufferIDs[16];
	unsigned int initialVerts = 0;
	unsigned int initialFaces = 0;
	unsigned int numFaces = 0;
	bool textured = false;
	std::string materialName = "";
	std::string name = "";

	uMesh();

	void addVert(const glm::vec3 &v);
	void addVert(const float &x, const float &y, const float &z);
	void addFace(unsigned const int &indexA, unsigned const int &indexB, unsigned const int &indexC);
	void build();
	void rebuild();
	void display();
	void displayFFP();
	void printData();
	void removeDoubles(bool pos, bool norms, bool tex);
	void loadOBJRAW(const char* location, bool merge);
	bool isEqual(unsigned const int &indexA, unsigned const int &indexB, bool pos, bool norm, bool tex);
	void mergeVerts(unsigned const int &indexA, unsigned const int &indexB);
	void resetSize();
	void subDiv(float displaceFactor, glm::vec3 position, glm::mat4 modelViewMatrix, glm::mat4 projMatrix);
};

#endif