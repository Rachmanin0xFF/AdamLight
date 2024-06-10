#include "uMaterial.h"


#include <GL/glew.h>  
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <iostream>

#include "GLUtil.h"

uMaterial::uMaterial() {
	
}

void uMaterial::apply() {
	if (use_albedo_texture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, albedo_texture);
		uSetUniform1fNoSwitch("albedo_texture_on", 1.f);
	} else {
		glDisable(GL_TEXTURE0);
		uSetUniform1fNoSwitch("albedo_texture_on", 0.f);
	}

	if (use_normal_texture) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, normal_texture);
		uSetUniform1fNoSwitch("normal_texture_on", 1.f);
	} else {
		glDisable(GL_TEXTURE2);
		uSetUniform1fNoSwitch("normal_texture_on", 0.f);
	}
	glActiveTexture(GL_TEXTURE0);

	uSetUniform1fNoSwitch("ambient_intensity", ambient_intensity);

	uSetUniform1fNoSwitch("specular_intensity", specular_intensity);
	uSetUniform3fNoSwitch("specular_color", specular_color);
	uSetUniform1fNoSwitch("hardness", hardness);

	uSetUniform1fNoSwitch("diffuse_intensity", diffuse_intensity);
	uSetUniform3fNoSwitch("diffuse_color", diffuse_color);

	glEnable(GL_TEXTURE0);
	glEnable(GL_TEXTURE2);
}

void uMaterial::printInfo() {
	std::cout << "Printing information of material " << name << std::endl;
	std::cout << "     use_albedo_texture - " << use_albedo_texture << std::endl;
	std::cout << "     use_normal_texture - " << use_normal_texture << std::endl;
	if (use_albedo_texture)
		std::cout << "         albedo texture - " << albedo_texture << std::endl;
	if (use_normal_texture)
		std::cout << "         normal texture - " << normal_texture << std::endl;
	std::cout << "      diffuse_intensity - " << diffuse_intensity << std::endl;
	std::cout << "      ambient_intensity - " << ambient_intensity << std::endl;
	std::cout << "          diffuse_color - " << "(" << diffuse_color.x << ", " << diffuse_color.y << ", " << diffuse_color.z << ")" << std::endl;
	std::cout << "     specular_intensity - " << specular_intensity << std::endl;
	std::cout << "         specular_color - " << "(" << specular_color.x << ", " << specular_color.y << ", " << specular_color.z << ")" << std::endl;
	std::cout << "     hardness - " << hardness << std::endl;
}