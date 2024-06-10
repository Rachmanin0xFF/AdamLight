#define _CRT_SECURE_NO_DEPRECATE

#include "uModel.h"

#include <vector>
#include "uMaterial.h"
#include "uMesh.h"
#include "GLUtil.h"
#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <algorithm>

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

std::vector<float> splitToFloats(const std::string &s, char delim, int indexoff) {
	std::vector<std::string> elems;
	std::vector<float> float_elems;
	split(s, delim, elems);
	for (int i = indexoff; i < elems.size(); i++)
		float_elems.push_back(std::stof(elems[i]));
	return float_elems;
}

std::vector<int> splitToInts(const std::string &s, char delim, int indexoff) {
	std::vector<std::string> elems;
	std::vector<int> int_elems;
	split(s, delim, elems);
	for (int i = indexoff; i < elems.size(); i++)
	if (elems[i] != "")
		int_elems.push_back(std::stoi(elems[i]));
	return int_elems;
}

uModel::uModel() {}

void uModel::render() {
	for (int i = 0; i < meshes.size(); i++) {
		materials[meshes[i].materialName].apply();
		meshes[i].display();
	}
}

void uModel::loadModelOBJ(const char* location) {
	std::cout << "Loading .OBJ multi-object (model) file from: " << location << std::endl;

	std::ifstream obj_file(location);
	std::string line;
	if (!obj_file.is_open()) {
		std::cout << "Could not open .OBJ file " << location << std::endl;
		return;
	}

	uMesh m;
	std::vector<glm::vec3> tempVerts;
	std::vector<glm::vec3> tempNorms;
	std::vector<glm::vec2> tempUVs;
	bool textured = false;
	bool first_obj = true;
	int numFaces = 0;

	while (std::getline(obj_file, line)) {
		if (line.compare(0, 2, "v ") == 0) {
			glm::vec3 v;
			std::vector<float> d = splitToFloats(line, ' ', 1);
			v = glm::vec3(d[0], d[1], d[2]);
			tempVerts.push_back(v);
		}
		else if (line.compare(0, 3, "vn ") == 0) {
			glm::vec3 v;
			std::vector<float> d = splitToFloats(line, ' ', 1);
			v = glm::vec3(d[0], d[1], d[2]);
			tempNorms.push_back(v);
		}
		else if (line.compare(0, 3, "vt ") == 0) {
			glm::vec2 v;
			std::vector<float> d = splitToFloats(line, ' ', 1);
			v = glm::vec2(d[0], d[1]);
			tempUVs.push_back(v);
			textured = true;
		}
		else if (line.compare(0, 2, "o ") == 0) {
			if (first_obj) {
				m.initialVerts = m.stream[0].size();
				m.initialFaces = m.faces.size();
				m.build();
				meshes.push_back(m);
				m = uMesh();
				textured = false;
				numFaces = 0;
			}
			else
				first_obj = false;
			m.name = line.substr(2);
		}
		else if (line.compare(0, 7, "usemtl ") == 0) {
			m.materialName = line.substr(7);
		}
		else if (line.compare(0, 7, "mtllib ") == 0) {
			std::vector<std::string> location_split = split(location, '/');
			std::string floc = "";
			for (int i = 0; i < location_split.size() - 1; i++) {
				floc += location_split[i] + "/";
			}
			floc += line.substr(7);
			loadMaterials(floc.c_str());
		}
		else if (textured) {
			if (line.compare(0, 2, "f ") == 0) {
				int a1, a2, a3;
				int b1, b2, b3;
				int c1, c2, c3;

				std::vector<std::string> d = split(line, ' ');
				std::vector<float> a = splitToFloats(d[1], '/', 0);
				std::vector<float> b = splitToFloats(d[2], '/', 0);
				std::vector<float> c = splitToFloats(d[3], '/', 0);
				a1 = a[0]; a2 = a[1]; a3 = a[2];
				b1 = b[0]; b2 = b[1]; b3 = b[2];
				c1 = c[0]; c2 = c[1]; c3 = c[2];

				m.stream[0].push_back(glm::vec3(tempVerts[a1 - 1].x, tempVerts[a1 - 1].y, tempVerts[a1 - 1].z));
				m.stream[0].push_back(glm::vec3(tempVerts[b1 - 1].x, tempVerts[b1 - 1].y, tempVerts[b1 - 1].z));
				m.stream[0].push_back(glm::vec3(tempVerts[c1 - 1].x, tempVerts[c1 - 1].y, tempVerts[c1 - 1].z));

				m.stream[8].push_back(glm::vec3(tempUVs[a2 - 1].x, tempUVs[a2 - 1].y, 0.0));
				m.stream[8].push_back(glm::vec3(tempUVs[b2 - 1].x, tempUVs[b2 - 1].y, 0.0));
				m.stream[8].push_back(glm::vec3(tempUVs[c2 - 1].x, tempUVs[c2 - 1].y, 0.0));

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
				m.stream[9].push_back(glm::vec3(tangent));
				m.stream[9].push_back(glm::vec3(tangent));
				m.stream[9].push_back(glm::vec3(tangent));
				m.stream[10].push_back(glm::vec3(bitangent));
				m.stream[10].push_back(glm::vec3(bitangent));
				m.stream[10].push_back(glm::vec3(bitangent));

				m.stream[2].push_back(glm::vec3(tempNorms[a3 - 1].x, tempNorms[a3 - 1].y, tempNorms[a3 - 1].z));
				m.stream[2].push_back(glm::vec3(tempNorms[b3 - 1].x, tempNorms[b3 - 1].y, tempNorms[b3 - 1].z));
				m.stream[2].push_back(glm::vec3(tempNorms[c3 - 1].x, tempNorms[c3 - 1].y, tempNorms[c3 - 1].z));
				m.faces.push_back(uFace(numFaces * 3, numFaces * 3 + 1, numFaces * 3 + 2));
				numFaces++;
			}
		}
		else {
			if (line.compare(0, 2, "f ") == 0) {
				int a1, a2;
				int b1, b2;
				int c1, c2;
				std::vector<std::string> d = split(line, ' ');

				std::vector<int> a = splitToInts(d[1], '/', 0);
				std::vector<int> b = splitToInts(d[2], '//', 0);
				std::vector<int> c = splitToInts(d[3], '//', 0);
				a1 = a[0]; a2 = a[1];
				b1 = b[0]; b2 = b[1];
				c1 = c[0]; c2 = c[1];

				m.stream[0].push_back(glm::vec3(tempVerts[a1 - 1].x, tempVerts[a1 - 1].y, tempVerts[a1 - 1].z));
				m.stream[0].push_back(glm::vec3(tempVerts[b1 - 1].x, tempVerts[b1 - 1].y, tempVerts[b1 - 1].z));
				m.stream[0].push_back(glm::vec3(tempVerts[c1 - 1].x, tempVerts[c1 - 1].y, tempVerts[c1 - 1].z));

				m.stream[2].push_back(glm::vec3(tempNorms[a2 - 1].x, tempNorms[a2 - 1].y, tempNorms[a2 - 1].z));
				m.stream[2].push_back(glm::vec3(tempNorms[b2 - 1].x, tempNorms[b2 - 1].y, tempNorms[b2 - 1].z));
				m.stream[2].push_back(glm::vec3(tempNorms[c2 - 1].x, tempNorms[c2 - 1].y, tempNorms[c2 - 1].z));
				m.faces.push_back(uFace(numFaces * 3, numFaces * 3 + 1, numFaces * 3 + 2));
				numFaces++;

			}
		}
	}
	m.initialVerts = m.stream[0].size();
	m.initialFaces = m.faces.size();
	m.build();
	meshes.push_back(m);

	std::cout << "Materials loaded." << std::endl;
	obj_file.close();

}

void uModel::loadModelALM(const char* location) {
	std::cout << "Loading .OBJ multi-object (model) file from: " << location << std::endl;

	std::ifstream obj_file(location);
	std::string line;
	if (!obj_file.is_open()) {
		std::cout << "Could not open .OBJ file " << location << std::endl;
		return;
	}

	uMesh m;
	std::vector<glm::vec3> tempVerts;
	std::vector<glm::vec3> tempNorms;
	std::vector<glm::vec2> tempUVs;
	bool textured = false;
	bool first_obj = true;
	int numFaces = 0;

	while (std::getline(obj_file, line)) {
		if (line.compare(0, 2, "q ") == 0) {
			glm::vec3 v;
			glm::vec2 vt;
			glm::vec3 vn;
			std::vector<float> d = splitToFloats(line, ' ', 1);
			v = glm::vec3(d[0], d[1], d[2]);
			vt = glm::vec2(d[3], d[4]);
			vn = glm::vec3(d[5], d[6], d[7]);
			tempVerts.push_back(v);
			tempUVs.push_back(vt);
			tempNorms.push_back(vn);
		}
		else if (line.compare(0, 7, "usemtl ") == 0) {
			m.materialName = line.substr(7);
		}
		else if (line.compare(0, 7, "mtllib ") == 0) {
			std::vector<std::string> location_split = split(location, '/');
			std::string floc = "";
			for (int i = 0; i < location_split.size() - 1; i++) {
				floc += location_split[i] + "/";
			}
			floc += line.substr(7);
			loadMaterials(floc.c_str());
		}
		else if (textured) {
			if (line.compare(0, 2, "f ") == 0) {
				
				numFaces++;
			}
		}
		else {
			if (line.compare(0, 2, "f ") == 0) {
				/*
				int a1, a2;
				int b1, b2;
				int c1, c2;
				std::vector<std::string> d = split(line, ' ');

				std::vector<int> a = splitToInts(d[1], '/', 0);
				std::vector<int> b = splitToInts(d[2], '/', 0);
				std::vector<int> c = splitToInts(d[3], '/', 0);
				a1 = a[0]; a2 = a[1];
				b1 = b[0]; b2 = b[1];
				c1 = c[0]; c2 = c[1];

				m.stream[0].push_back(glm::vec3(tempVerts[a1 - 1].x, tempVerts[a1 - 1].y, tempVerts[a1 - 1].z));
				m.stream[0].push_back(glm::vec3(tempVerts[b1 - 1].x, tempVerts[b1 - 1].y, tempVerts[b1 - 1].z));
				m.stream[0].push_back(glm::vec3(tempVerts[c1 - 1].x, tempVerts[c1 - 1].y, tempVerts[c1 - 1].z));

				m.stream[2].push_back(glm::vec3(tempNorms[a2 - 1].x, tempNorms[a2 - 1].y, tempNorms[a2 - 1].z));
				m.stream[2].push_back(glm::vec3(tempNorms[b2 - 1].x, tempNorms[b2 - 1].y, tempNorms[b2 - 1].z));
				m.stream[2].push_back(glm::vec3(tempNorms[c2 - 1].x, tempNorms[c2 - 1].y, tempNorms[c2 - 1].z));
				m.faces.push_back(uFace(numFaces * 3, numFaces * 3 + 1, numFaces * 3 + 2));
				numFaces++;
				*/
			}
		}
	}
	m.initialVerts = m.stream[0].size();
	m.initialFaces = m.faces.size();
	m.build();
	meshes.push_back(m);

	std::cout << "Materials loaded." << std::endl;
	obj_file.close();

}

void uModel::loadMaterials(const char* location) {
	std::cout << "Loading .MTL multi-material file from: " << location << std::endl;

	std::ifstream mtl_file(location);
	std::string line;
	if (!mtl_file.is_open()) {
		std::cout << "Could not open .MTL file " << location << std::endl;
		return;
	}
	bool first_mtl = true;
	uMaterial current_material;
	while (std::getline(mtl_file, line)) {
		if (line.find("newmtl") == 0) {
			if (!first_mtl) {
				current_material.printInfo();
				materials[current_material.name] = current_material;
				current_material = uMaterial();
			}
			else
				first_mtl = false;
			current_material.name = line.substr(7);
		}

		if (line.find("Ns") == 0) {
			current_material.hardness = ((std::stof(line.substr(3)) / 1.96078431373) + 1.f);
		}
		if (line.find("Ka") == 0) {
			std::vector<std::string> d = split(line, ' ');
			float r = std::stof(d[1]);
			float g = std::stof(d[2]);
			float b = std::stof(d[3]);
			current_material.ambient_intensity = (r + g + b) / 3.f;
		}
		if (line.find("Kd") == 0) {
			std::vector<std::string> d = split(line, ' ');
			float r = std::stof(d[1]);
			float g = std::stof(d[2]);
			float b = std::stof(d[3]);
			float div = fmax(fmax(r, g), b);
			float r2 = r / div;
			float g2 = g / div;
			float b2 = b / div;
			current_material.diffuse_color = glm::vec3(r2, g2, b2);
			current_material.diffuse_intensity = div;
		}
		if (line.find("Ks") == 0) {
			std::vector<std::string> d = split(line, ' ');
			float r = std::stof(d[1]);
			float g = std::stof(d[2]);
			float b = std::stof(d[3]);
			float div = fmax(fmax(r, g), b);
			float r2 = r / div;
			float g2 = g / div;
			float b2 = b / div;
			current_material.specular_color = glm::vec3(r2, g2, b2);
			current_material.specular_intensity = div;
		}
		if (line.find("map_Kd") == 0) {

			//std::vector<std::string> location_split = split(location, '/');
			//std::string floc = "";
			//for (int i = 0; i < location_split.size() - 1; i++) {
			//	floc += location_split[i] + "/";
			//}
			//floc += line.substr(7);
			//loadMaterials(floc.c_str());
			current_material.albedo_texture = uLoadBMP(line.substr(7).c_str());
			current_material.use_albedo_texture = true;
		}
		if (line.find("map_Bump") == 0) {
			if (line.find("C:") == 9) {
				current_material.normal_texture = uLoadBMP(line.substr(9).c_str());
			}
			else {
				std::vector<std::string> location_split = split(location, '/');
				std::string floc = "";
				for (int i = 0; i < location_split.size() - 1; i++) {
					floc += location_split[i] + "/";
				}
				floc += line.substr(9);
				current_material.normal_texture = uLoadBMP(floc.c_str());
			}

			current_material.use_normal_texture = true;
		}
	}
	materials[current_material.name] = current_material;
	mtl_file.close();
}
