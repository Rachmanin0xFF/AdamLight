#version 120
uniform sampler2D TA;
void main(void) {
	gl_FragColor.rgb = abs(sin(1000.0 * texture2D(TA, gl_TexCoord[0].st).rgb));
}