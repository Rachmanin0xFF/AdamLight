#version 330 core
uniform float zNear;
uniform float zFar;
uniform float ortho;
out vec4 fragmentOut;
in float Z;

float linDepth(float zval) {
    float z_b = zval;
    float z_n = 2.0 * z_b - 1.0;
    return zNear * zFar/(zFar + zNear - z_n * (zFar - zNear)) * 2.0;
}

void main(void) {
	if(ortho > 0.5) {
		fragmentOut.r = Z;
		gl_FragDepth = (Z+zFar)/2.0/zFar;
	}
	if(ortho < 0.5) {
		fragmentOut.r = (linDepth(gl_FragCoord.z) - zNear) / (zFar - zNear);
		gl_FragDepth = gl_FragCoord.z;
	}
}