
#ifndef UMODEL_H
#define UMODEL_H

#include <vector>
#include "uMaterial.h"
#include "uMesh.h"
#include <string>
#include <map>

class uModel {
public:
	std::vector<uMesh> meshes;
	std::map<std::string, uMaterial> materials;
	uModel();
	void loadModelOBJ(const char* location);
	void loadModelALM(const char* location);
	void loadMaterials(const char* location);
	void render();
};

#endif