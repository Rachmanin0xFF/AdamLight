#version 120

uniform sampler2D TA;
void main(void) {
	gl_FragColor = texture2D(TA, gl_TexCoord[0].st);
}