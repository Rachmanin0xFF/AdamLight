#version 120
uniform sampler2D TA;
void main(void) {
	float z = abs(sin(texture2D(TA, gl_TexCoord[0].st).r * 100.0));
	gl_FragColor = vec4(z, z, z, 1.0);
}