#version 120

uniform sampler2D TA;
void main(void) {
	float gam = 9.2;
	gl_FragColor.rgb = pow(texture2D(TA, gl_TexCoord[0].st).rgb, vec3(gam, gam, gam));
	gl_FragColor.a = 1.0;
}