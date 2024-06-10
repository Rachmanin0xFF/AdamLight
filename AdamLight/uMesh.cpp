#define _CRT_SECURE_NO_DEPRECATE

//--------------------------------------------------------------------//
#include <GL/GLEW.h>

#include "uMesh.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

//--------------------------------------------------------------------//

//int x = index_of(arr, arr+5, valueToFind)
template <typename Iter>
size_t index_of(Iter first, Iter last, typename const std::iterator_traits<Iter>::value_type& x) {
	size_t i = 0;
	while (first != last && *first != x)
		++first, ++i;
	return i;
}

unsigned intPower(unsigned val, unsigned _pow = 0) {
	if (_pow <= 0)
		return 1;
	return val * intPower(val, _pow - 1);
}

float getRand(const float &a, const float &b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

glm::vec3 getRandVec(const float &r) {
	return glm::vec3(getRand(-r, r), getRand(-r, r), getRand(-r, r));
}

//--------------------------------------------------------------------//

void removeVec(std::vector<glm::vec3> &pList, const int &i) {
	pList.erase(pList.begin() + i);
}

//--------------------------------------------------------------------//

uFace::uFace() {
	show = true;
}

uFace::uFace(unsigned int indexA, unsigned int indexB, unsigned int indexC) {
	verts[0] = indexA;
	verts[1] = indexB;
	verts[2] = indexC;
}

//--------------------------------------------------------------------//

uMesh::uMesh(){}

void uMesh::build() {
	std::vector<unsigned int> indx;
	for (int i = 0; i < faces.size(); i++) {
		if (faces[i].show) {
			indx.push_back(faces[i].verts[0]);
			indx.push_back(faces[i].verts[1]);
			indx.push_back(faces[i].verts[2]);
		}
	}
	numFaces = indx.size();
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indx.size() * sizeof(unsigned int), &indx[0], GL_STATIC_DRAW);

	for (int i = 0; i < 16; i++) {
		if (stream[i].size() > 0) {
			glGenBuffers(1, &bufferIDs[i]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIDs[i]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, stream[i].size() * sizeof(glm::vec3), &stream[i][0], GL_STATIC_DRAW);
		}
	}
}

void uMesh::rebuild() {
	std::vector<unsigned int> indx;
	for (int i = 0; i < faces.size(); i++) {
		if (faces[i].show) {
			indx.push_back(faces[i].verts[0]);
			indx.push_back(faces[i].verts[1]);
			indx.push_back(faces[i].verts[2]);
		}
	}
	numFaces = indx.size();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indx.size() * sizeof(unsigned int), &indx[0], GL_STATIC_DRAW);

	for (int i = 0; i < 16; i++) {
		if (stream[i].size() > 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIDs[i]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, stream[i].size() * sizeof(glm::vec3), &stream[i][0], GL_STATIC_DRAW);
		}
	}
}

void uMesh::display() {
	for (int i = 0; i < 16; i++) {
		if (stream[i].size() > 0) {
			glEnableVertexAttribArray(i);
			glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[i]);
			glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glDrawElements(GL_TRIANGLES, numFaces, GL_UNSIGNED_INT, (void*)0);

	for (int i = 0; i < 16; i++)
		if (stream[i].size() > 0)
			glDisableVertexAttribArray(i);
}

void uMesh::displayFFP() {
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < faces.size(); i++) {
		if (faces[i].show) {
			if (stream[8].size()>0)
				glTexCoord2f(stream[8][faces[i].verts[0]].x, stream[8][faces[i].verts[0]].y);
			if (stream[2].size()>0)
				glNormal3f(stream[2][faces[i].verts[0]].x, stream[2][faces[i].verts[0]].y, stream[2][faces[i].verts[0]].z);
			glVertex3f(stream[0][faces[i].verts[0]].x, stream[0][faces[i].verts[0]].y, stream[0][faces[i].verts[0]].z);

			if (stream[8].size()>0)
				glTexCoord2f(stream[8][faces[i].verts[1]].x, stream[8][faces[i].verts[1]].y);
			if (stream[2].size()>0)
				glNormal3f(stream[2][faces[i].verts[1]].x, stream[2][faces[i].verts[1]].y, stream[2][faces[i].verts[1]].z);
			glVertex3f(stream[0][faces[i].verts[1]].x, stream[0][faces[i].verts[1]].y, stream[0][faces[i].verts[1]].z);

			if (stream[8].size()>0)
				glTexCoord2f(stream[8][faces[i].verts[2]].x, stream[8][faces[i].verts[2]].y);
			if (stream[2].size()>0)
				glNormal3f(stream[2][faces[i].verts[2]].x, stream[2][faces[i].verts[2]].y, stream[2][faces[i].verts[2]].z);
			glVertex3f(stream[0][faces[i].verts[2]].x, stream[0][faces[i].verts[2]].y, stream[0][faces[i].verts[2]].z);
		}
	}

	glEnd();
}

void uMesh::printData() {
	using namespace std;
	cout << "MESH DATA:" << endl;
	cout << "  FACES:" << endl;
	for (int i = 0; i < faces.size(); i++) {
		cout << "    [" << i << "] - " << faces[i].verts[0] << " " << faces[i].verts[1] << " " << faces[i].verts[2] << " SHOW: ";
		printf(faces[i].show ? "true\n" : "false\n");
	}
	for (int i = 0; i < 16; i++) {
		if (stream[i].size() > 0) {
			cout << "  STREAM[" << i << "]" << std::endl;
			for (int k = 0; k < stream[i].size(); k++)
				cout << "    [" << k << "] - " << " X:" << stream[i][k].x << " Y:" << stream[i][k].y << " Z:" << stream[i][k].z << std::endl;
		}
	}
}

void uMesh::removeDoubles(bool pos, bool norms, bool tex) {
	for (int i = 0; i < stream[0].size(); i++) {
		for (int k = stream[0].size() - 1; k > 0; k--) {
			if ((k > i) && (i != k) && isEqual(i, k, pos, norms, tex)) {
				mergeVerts(i, k);
			}
		}
	}
}

void uMesh::loadOBJRAW(const char* location, bool merge) {
	int numVerts = 0;
	int numFaces = 0;
	std::cout << "Loading .OBJ single-object file from " << location << std::endl;
	std::vector<glm::vec3> tempVerts;
	std::vector<glm::vec3> tempNorms;
	std::vector<glm::vec2> tempUVs;
	FILE * file = fopen(location, "r");
	if (file == NULL){
		fprintf(stdout, "File could not be opened!\n");
	}
	else {
		bool useTex;

		useTex = false;

		std::cout << "File opened, reading..." << std::endl;
		while (true) {

			char header[128];
			int r = fscanf(file, "%s", header);

			if (r == EOF)
				break;
			
			if (strcmp(header, "v") == 0) {
				glm::vec3 v;
				fscanf(file, "%f %f %f\n", &v.x, &v.y, &v.z);
				tempVerts.push_back(v);
				numVerts++;
			}

			if (strcmp(header, "vn") == 0) {
				glm::vec3 n;
				fscanf(file, "%f %f %f\n", &n.x, &n.y, &n.z);
				tempNorms.push_back(n);
			}

			if (strcmp(header, "vt") == 0) {
				glm::vec2 t;
				fscanf(file, "%f %f\n", &t.x, &t.y);
				tempUVs.push_back(t);
				textured = true;
			}

			if (textured) {
				if (strcmp(header, "f") == 0) {
					int a1, a2, a3;
					int b1, b2, b3;
					int c1, c2, c3;
					fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &a1, &a2, &a3, &b1, &b2, &b3, &c1, &c2, &c3);
					stream[0].push_back(glm::vec3(tempVerts[a1 - 1].x, tempVerts[a1 - 1].y, tempVerts[a1 - 1].z));
					stream[0].push_back(glm::vec3(tempVerts[b1 - 1].x, tempVerts[b1 - 1].y, tempVerts[b1 - 1].z));
					stream[0].push_back(glm::vec3(tempVerts[c1 - 1].x, tempVerts[c1 - 1].y, tempVerts[c1 - 1].z));

					stream[8].push_back(glm::vec3(tempUVs[a2 - 1].x, tempUVs[a2 - 1].y, 0.0));
					stream[8].push_back(glm::vec3(tempUVs[b2 - 1].x, tempUVs[b2 - 1].y, 0.0));
					stream[8].push_back(glm::vec3(tempUVs[c2 - 1].x, tempUVs[c2 - 1].y, 0.0));

					//Computing tangents and bitangents:

					//Get data out of arrays and into variables to make the code more readable.
					glm::vec3 &v0 = tempVerts[a1 - 1];
					glm::vec3 &v1 = tempVerts[b1 - 1];
					glm::vec3 &v2 = tempVerts[c1 - 1];
					glm::vec2 &uv0 = tempUVs[a2 - 1];
					glm::vec2 &uv1 = tempUVs[b2 - 1];
					glm::vec2 &uv2 = tempUVs[c2 - 1];

					//Compute triangle edges (position delta)
					glm::vec3 deltaPos1 = v1 - v0;
					glm::vec3 deltaPos2 = v2 - v0;

					//Delta-uv
					glm::vec2 deltaUV1 = uv1 - uv0;
					glm::vec2 deltaUV2 = uv2 - uv0;

					//Compute matrix inverse (or transpose, due to it's orthogonality).
					float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
					glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
					glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

					//Add each vector 3 times for each 3 vertices.
					stream[9].push_back(glm::vec3(tangent));
					stream[9].push_back(glm::vec3(tangent));
					stream[9].push_back(glm::vec3(tangent));
					stream[10].push_back(glm::vec3(bitangent));
					stream[10].push_back(glm::vec3(bitangent));
					stream[10].push_back(glm::vec3(bitangent));

					stream[2].push_back(glm::vec3(tempNorms[a3 - 1].x, tempNorms[a3 - 1].y, tempNorms[a3 - 1].z));
					stream[2].push_back(glm::vec3(tempNorms[b3 - 1].x, tempNorms[b3 - 1].y, tempNorms[b3 - 1].z));
					stream[2].push_back(glm::vec3(tempNorms[c3 - 1].x, tempNorms[c3 - 1].y, tempNorms[c3 - 1].z));
					faces.push_back(uFace(numFaces * 3, numFaces * 3 + 1, numFaces * 3 + 2));
					numFaces++;
				}
			}
			else {
				if (strcmp(header, "f") == 0) {
					int a1, a2;
					int b1, b2;
					int c1, c2;
					fscanf(file, "%d//%d %d//%d %d//%d\n", &a1, &a2, &b1, &b2, &c1, &c2);
					stream[0].push_back(glm::vec3(tempVerts[a1 - 1].x, tempVerts[a1 - 1].y, tempVerts[a1 - 1].z));
					stream[0].push_back(glm::vec3(tempVerts[b1 - 1].x, tempVerts[b1 - 1].y, tempVerts[b1 - 1].z));
					stream[0].push_back(glm::vec3(tempVerts[c1 - 1].x, tempVerts[c1 - 1].y, tempVerts[c1 - 1].z));

					stream[2].push_back(glm::vec3(tempNorms[a2 - 1].x, tempNorms[a2 - 1].y, tempNorms[a2 - 1].z));
					stream[2].push_back(glm::vec3(tempNorms[b2 - 1].x, tempNorms[b2 - 1].y, tempNorms[b2 - 1].z));
					stream[2].push_back(glm::vec3(tempNorms[c2 - 1].x, tempNorms[c2 - 1].y, tempNorms[c2 - 1].z));
					faces.push_back(uFace(numFaces * 3, numFaces * 3 + 1, numFaces * 3 + 2));
					numFaces++;
				}
			}
		}

		if (merge) {
			std::cout << "Merging vertices..." << std::endl;
			int tempNumVerts = stream[0].size();
			removeDoubles(true, true, textured);
			std::cout << tempNumVerts - stream[0].size() << " vertices merged. Compression ratio at " << static_cast<float>(stream[0].size()) / static_cast<float>(tempNumVerts) * 100.0f << "%" << std::endl;
		}

		initialVerts = stream[0].size();
		initialFaces = faces.size();

		build();

		std::cout << "File loaded successfully with " << stream[0].size() << " vertices and " << faces.size() << " faces.\n" << std::endl;
	}
}

bool uMesh::isEqual(unsigned const int &indexA, unsigned const int &indexB, bool pos, bool norm, bool tex) {
	glm::vec3 posA = stream[0][indexA];
	glm::vec3 posB = stream[0][indexB];
	glm::vec3 normA = stream[2][indexA];
	glm::vec3 normB = stream[2][indexB];
	glm::vec3 texA;
	glm::vec3 texB;
	if (tex) {
		texA = stream[8][indexA];
		texB = stream[8][indexB];
	}

	float posDiff = glm::length(posA - posB);
	float normDiff = glm::length(normA - normB);
	float texDiff = glm::length(texA - texB);

	if (!pos) posDiff = 0.0f;
	if (!norm) normDiff = 0.0f;
	if (!tex) texDiff = 0.0f;

	return (posDiff == 0.0f) && (normDiff == 0.0f) && (texDiff == 0.0f);
}

void uMesh::mergeVerts(unsigned const int &indexA, unsigned const int &indexB) {
	if (indexB > indexA) {
		for (int i = 0; i < 16; i++) {
			if (stream[i].size() > indexB) {
				stream[i][indexA] = stream[i][indexB];
				removeVec(stream[i], indexB);
			}
		}
		for (int i = 0; i < faces.size(); i++) {
			for (int k = 0; k < 3; k++) {
				if (faces[i].verts[k] > indexB) faces[i].verts[k] = faces[i].verts[k] - 1;
				else if (faces[i].verts[k] == indexB) faces[i].verts[k] = indexA;
			}
		}
	}
}

void uMesh::resetSize() {
	if (faces.size() != initialFaces)
		faces.erase(faces.begin() + initialFaces, faces.begin() + faces.size());
	if (stream[0].size() != initialVerts)
		stream[0].erase(stream[0].begin() + initialVerts, stream[0].begin() + stream[0].size());
	if (stream[2].size() != initialVerts)
		stream[2].erase(stream[2].begin() + initialVerts, stream[2].begin() + stream[2].size());
	if (textured && (stream[8].size() != initialVerts))
		stream[8].erase(stream[8].begin() + initialVerts, stream[8].begin() + stream[8].size());
	for (int i = 0; i < faces.size(); i++)
		faces[i].show = true;
}

void uMesh::subDiv(float displaceFactor, glm::vec3 position, glm::mat4 modelViewMatrix, glm::mat4 projMatrix) {
	resetSize();

	float r = 10000.0f;
	float heightMul = 0.075f * displaceFactor;

	for (float k = 0.0f; k < 15.0f; k++)
	for (int i = faces.size() - 1; i >= 0; i--) {

		float a = glm::distance(stream[0][faces[i].verts[0]], stream[0][faces[i].verts[1]]);
		float b = glm::distance(stream[0][faces[i].verts[1]], stream[0][faces[i].verts[2]]);
		float c = glm::distance(stream[0][faces[i].verts[2]], stream[0][faces[i].verts[0]]);
		float p = (a + b + c) / 2.0f;

		glm::vec3 avg = (stream[0][faces[i].verts[0]] + stream[0][faces[i].verts[1]] + stream[0][faces[i].verts[2]]) / 3.0f;

		//////////////////////////////////////////////////////////////////////////////////////////

		if (faces[i].show && a*4 < glm::distance(position, avg)) {
			glm::vec4 viewed = modelViewMatrix * glm::vec4(avg, 1.0f);
			if (viewed.z < 0.0) {
				glm::vec4 projected = projMatrix * viewed;
				projected /= projected.w;
				if (projected.x < -1.2f || projected.y < -1.2f || projected.x > 1.2f || projected.y > 1.2f)
					faces[i].show = false;
			} else
				faces[i].show = false;
		}
		
		//////////////////////////////////////////////////////////////////////////////////////////

		if (faces[i].show && p / glm::distance(avg, position) > 0.1f) {
			
			int vertIndex = stream[0].size();

			glm::vec3 nv0 = (stream[0][faces[i].verts[0]] + stream[0][faces[i].verts[1]]) / 2.0f;
			glm::vec3 nv1 = (stream[0][faces[i].verts[1]] + stream[0][faces[i].verts[2]]) / 2.0f;
			glm::vec3 nv2 = (stream[0][faces[i].verts[2]] + stream[0][faces[i].verts[0]]) / 2.0f;

			float height = heightMul / intPower(2, k);
			float h1 = heightMul * glm::distance(stream[0][faces[i].verts[0]], stream[0][faces[i].verts[1]]);
			float h2 = heightMul * glm::distance(stream[0][faces[i].verts[1]], stream[0][faces[i].verts[2]]);
			float h3 = heightMul * glm::distance(stream[0][faces[i].verts[2]], stream[0][faces[i].verts[0]]);

			srand(176501531.7f * (nv0.x + nv0.y + nv0.z + 4554));
			glm::vec3 r1 = getRandVec(h1) + nv0;
			srand(176501531.7f * (nv1.x + nv1.y + nv1.z + 4554));
			glm::vec3 r2 = getRandVec(h2) + nv1;
			srand(176501531.7f * (nv2.x + nv2.y + nv2.z + 4554));
			glm::vec3 r3 = getRandVec(h3) + nv2;

			nv0 = r1;
			nv1 = r2;
			nv2 = r3;

			stream[0].push_back(nv0); stream[0].push_back(nv1); stream[0].push_back(nv2);

			glm::vec3 nml(getRand(-1.0f, 1.0f), getRand(-1.0f, 1.0f), getRand(-1.0f, 1.0f));
			stream[2].push_back(nml); stream[2].push_back(nml); stream[2].push_back(nml);

			if (textured)
				stream[8].push_back(nml); stream[8].push_back(nml); stream[8].push_back(nml);


			faces.push_back(uFace(vertIndex, vertIndex + 2, faces[i].verts[0]));
			faces.push_back(uFace(vertIndex + 1, vertIndex, faces[i].verts[1]));
			faces.push_back(uFace(vertIndex + 2, vertIndex + 1, faces[i].verts[2]));
			faces.push_back(uFace(vertIndex, vertIndex + 1, vertIndex + 2));

			faces[i].show = false;
		}
	}

	for (int i = 0; i < stream[0].size(); i++) {
		if (glm::length(stream[0][i]) < r) {
			stream[2][i] = glm::vec3(0.0f, 0.2f, 1.0f);
			//stream[0][i] = r * glm::normalize(stream[0][i]);
		} else
			stream[2][i] = glm::vec3(0.5f, 1.0f, 0.1f);
	}

	rebuild();
}
//--------------------------------------------------------------------//