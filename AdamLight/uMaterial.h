#ifndef UMATERIAL_H
#define UMATERIAL_H

#include <GL/glew.h>  
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include <string>

class uMaterial {
public:
	std::string name;
	float specular_intensity = 0.0f;
	float diffuse_intensity = 0.0f;
	float ambient_intensity = 0.0f;
	float hardness = 0.0f;
	glm::vec3 diffuse_color;
	glm::vec3 specular_color;
	bool use_albedo_texture = false;
	bool use_normal_texture = false;
	GLuint albedo_texture;
	GLuint normal_texture;

	uMaterial();
	void apply();
	void printInfo();
};

#endif