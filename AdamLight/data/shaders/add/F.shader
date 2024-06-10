#version 120
uniform sampler2D TA;
uniform sampler2D TB;
void main(void) {
	gl_FragColor = texture2D(TA, gl_TexCoord[0].st) + texture2D(TB, gl_TexCoord[0].st);
}