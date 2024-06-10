#ifndef GLUTIL_H
#define GLUTIL_H

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

/**
Sets a uniform vertex attribute location to a OpenGL program (shader).
@param program The shader to set the value to.
@param name The name of the attribute inside the shader.
@param x The value to set.
*/
void uSetUniformVertexAttrib(int program, const char* name, int x);

/**
Sets a uniform integer to a OpenGL program (shader).
@param program The shader to set the value to.
@param name The name of the variable inside the shader.
@param x The value to set.
*/
void uSetUniform1i(int program, const char* name, int x);

/**
Sets a uniform float to a OpenGL program (shader).
@param program The shader to set the value to.
@param name The name of the variable inside the shader.
@param x The value to set.
*/
void uSetUniform1f(int program, const char* name, float x);

/**
Sets a uniform vec2 to a OpenGL program (shader).
@param program The shader to set the value to.
@param name The name of the variable inside the shader.
@param x The X value to set.
@param y The Y value to set.
*/
void uSetUniform2f(int program, const char* name, float x, float y);

/**
Sets a uniform vec2 to a OpenGL program (shader).
@param program The shader to set the value to.
@param name The name of the variable inside the shader.
@param v The value to set.
*/
void uSetUniform2f(int program, const char* name, glm::vec2 v);

/**
Sets a uniform vec3 to a OpenGL program (shader).
@param program The shader to set the value to.
@param name The name of the variable inside the shader.
@param x The X value to set.
@param y The Y value to set.
@param z The Z value to set.
*/
void uSetUniform3f(int program, const char* name, float x, float y, float z);

/**
Sets a uniform vec3 to a OpenGL program (shader).
@param program The shader to set the value to.
@param name The name of the variable inside the shader.
@param v The value to set.
*/
void uSetUniform3f(int program, const char* name, glm::vec3 v);

/**
Sets a uniform vec3 to a OpenGL program (shader).
@param program The shader to set the value to.
@param name The name of the variable inside the shader.
@param x The X value to set.
@param y The Y value to set.
@param z The Z value to set.
*/
void uSetUniform3fNoSwitch(const char* name, float x, float y, float z);

/**
Sets a uniform vec3 to a OpenGL program (shader).
@param program The shader to set the value to.
@param name The name of the variable inside the shader.
@param v The value to set.
*/
void uSetUniform3fNoSwitch(const char* name, glm::vec3 v);

/**
Sets a uniform matrix to a OpenGL program (shader).
@param program The shader to set the value to.
@param name The name of the variable inside the shader.
@param x The value to set.
*/
void uSetUniformMatrix3x3(int program, const char* name, glm::mat3 x);

/**
Sets a uniform matrix to a OpenGL program (shader).
@param program The shader to set the value to.
@param name The name of the variable inside the shader.
@param x The value to set.
*/
void uSetUniformMatrix4x4(int program, const char* name, glm::mat4 x);

void uSetUniform1iNoSwitch(const char* name, int x);
void uSetUniform1fNoSwitch(const char* name, float x);
void uSetUniformMatrix3x3NoSwitch(const char* name, glm::mat3 x);
void uSetUniformMatrix4x4NoSwitch(const char* name, glm::mat4 x);

/**
Prints a 4x4 glm matrix.
@param m The matrix to print.
*/
void uPrintMatrix(glm::mat4 m);

/**
Creates a new FrameBuffer Object.
Handles Are as follows-
[0]-FBOID
[1]-ColorTexture
@param out[] The array of length 2 containing the handles.
@param xres The X-Resolution of the FBO.
@param yres The Y-Resolution of the FBO.
*/
void uNewFBO_FCr(GLuint out[], unsigned const int &xres, unsigned const int &yres);

/**
Creates a new FrameBuffer Object.
Handles Are as follows-
[0]-FBOID
[1]-ColorTexture
@param out[] The array of length 2 containing the handles.
@param xres The X-Resolution of the FBO.
@param yres The Y-Resolution of the FBO.
*/
void uNewFBO_FHCr(GLuint out[], unsigned const int &xres, unsigned const int &yres);

/**
Creates a new FrameBuffer Object.
Handles Are as follows-
[0]-FBOID
[2]-DepthTexture
@param out[] The array of length 2 containing the handles.
@param xres The X-Resolution of the FBO.
@param yres The Y-Resolution of the FBO.
*/
void uNewFBO_FDr(GLuint out[], unsigned const int &xres, unsigned const int &yres);

/**
Creates a new FrameBuffer Object.
Handles Are as follows-
[0]-FBOID
[1]-ColorTexture
@param out[] The array of length 3 containing the handles.
@param xres The X-Resolution of the FBO.
@param yres The Y-Resolution of the FBO.
*/
void uNewFBO_FCrD(GLuint out[], unsigned const int &xres, unsigned const int &yres);

/**
Creates a new FrameBuffer Object.
Handles Are as follows-
[0]-FBOID
[1]-ColorTexture (GL_RGBA16F)
@param out[] The array of length 3 containing the handles.
@param xres The X-Resolution of the FBO.
@param yres The Y-Resolution of the FBO.
*/
void uNewFBO_FHCrD(GLuint out[], unsigned const int &xres, unsigned const int &yres);

/**
Creates a new FrameBuffer Object.
Handles Are as follows-
[0]-FBOID
[1]-ColorTexture
[2]-DepthTexture
@param out[] The array of length 3 containing the handles.
@param xres The X-Resolution of the FBO.
@param yres The Y-Resolution of the FBO.
*/
void uNewFBO_FHCrDr(GLuint out[], unsigned const int &xres, unsigned const int &yres);

/**
Creates a new FrameBuffer Object.
Handles Are as follows-
[0]-FBOID
[1]-ColorTexture
[2]-DepthTexture
@param out[] The array of length 3 containing the handles.
@param xres The X-Resolution of the FBO.
@param yres The Y-Resolution of the FBO.
*/
void uNewFBO_FCrDr(GLuint out[], unsigned const int &xres, unsigned const int &yres);

/**
Creates a new FrameBuffer Object.
Handles Are as follows-
[0]-FBOID
[1]-ColorTexture
[2]-DepthStencilTexture
@param out[] The array of length 3 containing the handles.
@param xres The X-Resolution of the FBO.
@param yres The Y-Resolution of the FBO.
*/
void uNewFBO_FCrDSr(GLuint out[], unsigned const int &xres, unsigned const int &yres);

/**
Creates a new FrameBuffer Object (usually a G-Buffer).
Handles Are as follows-
[0]-FBOID
[1]-ColorTexture
[2]-NormalTexture
[3]-DepthTexture
@param out[] The array of length 4 containing the handles.
@param xres The X-Resolution of the FBO.
@param yres The Y-Resolution of the FBO.
*/
void uNewFBO_FCrCrDr(GLuint out[], unsigned const int &xres, unsigned const int &yres);

/**
Creates a new FrameBuffer Object (usually a G-Buffer).
Handles Are as follows-
[0]-FBOID
[1]-ColorTexture
[2]-NormalTexture
[3]-DepthStencilTexture
@param out[] The array of length 4 containing the handles.
@param xres The X-Resolution of the FBO.
@param yres The Y-Resolution of the FBO.
*/
void uNewFBO_FCrCrDSr(GLuint out[], unsigned const int &xres, unsigned const int &yres);

/**
Creates a new FrameBuffer Object (usually a G-Buffer).
Handles Are as follows-
[0]-FBOID
[1]-ColorTexture
[2]-NormalTexture
[3]-DepthTexture
[4]-LightTexture
@param out[] The array of length 5 containing the handles.
@param xres The X-Resolution of the FBO.
@param yres The Y-Resolution of the FBO.
*/
void uNewFBO_FCrCrDrCr(GLuint out[], unsigned const int &xres, unsigned const int &yres);

/**
Creates a new FrameBuffer Object (usually a G-Buffer).
Handles Are as follows-
[0]-FBOID
[1]-ColorTexture
[2]-NormalTexture
[3]-DepthTexture
[4]-LightTexture
[5]-Misc. Lighting Data
@param out[] The array of length 5 containing the handles.
@param xres The X-Resolution of the FBO.
@param yres The Y-Resolution of the FBO.
*/
void uNewFBO_FCrCrDrCrCr(GLuint out[], unsigned const int &xres, unsigned const int &yres);

/**
Creates a new FrameBuffer Object (usually a G-Buffer).
Handles Are as follows-
[0]-FBOID
[1]-ColorTexture
[2]-NormalTexture
[3]-DepthStencilTexture
[4]-LightTexture
@param out[] The array of length 5 containing the handles.
@param xres The X-Resolution of the FBO.
@param yres The Y-Resolution of the FBO.
*/
void uNewFBO_FCrCrDSrCr(GLuint out[], unsigned const int &xres, unsigned const int &yres);

/**
Use this function to make the screen the render target.
*/
void uRenderToScreen();

/**
Draws a rectangle the size of the screen using the fixed-function pipeline.
*/
void uRectOnScreenFFP();

/**
Draws a rectangle the size of the screen without disabling the depth test using the fixed-function pipeline.
*/
void uRectOnScreenFFPWriteDepth();

/**
Draws a rectangle the size of the given dimensions using the fixed-function pipeline.
*/
void uRectOnScreenFFP(float rsX, float rsY);

/**
Draws a cube using the fixed-function pipeline.
@param x The x-coordinate of the cube.
@param y The y-coordinate of the cube.
@param z The z-coordinate of the cube.
*/
void uDrawCubeFFP(float x, float y, float z);

/**
Draws an octahedron using the fixed-function pipeline.
@param x The x-coordinate of the cube.
@param y The y-coordinate of the cube.
@param z The z-coordinate of the cube.
@param s The size of the octahedron.
*/
void uDrawOctahedronFFP(const float &x, const float &y, const float &z, const float &s);

/**
Loads a 24-bit .bmp file.
@param imagepath the path of the .bmp file.
*/
GLuint uLoadBMP(const char* imagepath);

/**
Loads a vertex + fragment shader. Not mine.
@param vertex_path The path of the vertex shader.
@param fragment_path the path of the fragment shader.
*/
GLuint uLoadShader(const char* vertex_path, const char* fragment_path);

/**
Switches the rendering target to a specified FBO.
@param FBOID The framebuffer I.D.
*/
void uRenderToFBO(int FBOID);

/**
Prints the current shader program in use using glGetIntegerv();
*/
void uPrintCurrentProgram();

/**
Filters a FBO to another FBO's depth texture through the use of glFragDepth. This is a specalized function, and does not set up all states before rendering.
@param FBOID The ID of the target FBO.
@param tex The texture to use (TA).
*/
void uFilterDepthWrite(int FBOID, int tex);

/**
"Filters" an FBO to another FBO.
@param FBOID The ID of the target FBO.
@param shader The shader to use when filtering.
@param xres The x-resolution of the target FBO.
@param yres The y-resolution of the target FBO.
@param ... The textures to use. In the shader these should be called TA, TB, TC, etc.
*/
void uFilterFBOToFBO(int FBOID, int shader, int xres, int yres, int texturesToUse, ...);

/**
"Filters" an FBO to another FBO without clearing it.
@param FBOID The ID of the target FBO.
@param shader The shader to use when filtering.
@param xres The x-resolution of the target FBO.
@param yres The y-resolution of the target FBO.
@param ... The textures to use. In the shader these should be called TA, TB, TC, etc.
*/
void uFilterFBOToFBONoClear(int FBOID, int shader, int xres, int yres, int texturesToUse, ...);

/**
"Filters" an FBO to another FBO without clearing it.
@param FBOID The ID of the target FBO.
@param shader The shader to use when filtering.
@param x0 The first argument of glViewport().
@param y0 The second argument of glViewport().
@param x1 The third argument of glViewport().
@param y1 The fourth argument of glViewport().
@param ... The textures to use. In the shader these should be called TA, TB, TC, etc.
*/
void uFilterFBOToFBONoClear(int FBOID, int shader, int x0, int y0, int x1, int y1, int texturesToUse, ...);

/**
Rounds a vec3 to the nearest supplied fraction. For example, if division is 0.15, it will round all components of v to the nearest multiple of 0.15.
@param v The vector to round.
@param divisionNum The fraction to round to.
*/
glm::vec3 uRoundVecTo(glm::vec3 v, float divisionNum);

/**
Multiplies a vec3 by a 4x4 matrix with a 1.0 in the W component of the vec3.
@param p the point to multiply.
@param matrix The matrix to use.
*/
glm::vec3 uMul3x4(glm::vec3 p, glm::mat4 matrix);

/**
Creates a rotation matrix to rotate a point to another point.
@param O The object's position.
@param P The position of the object to point to.
@param U The up vector.
*/
glm::mat4 uRotateToPoint(glm::vec3 O, glm::vec3 P, glm::vec3 U);


#endif