#version 120

uniform sampler2D TA;
uniform vec2 RES;

void main(void) {
	gl_FragColor += texture2DLod(TA, gl_TexCoord[0].st, 3);
	gl_FragColor += texture2DLod(TA, gl_TexCoord[0].st + vec2(1.5/RES.x, 0.0), 3);
	gl_FragColor += texture2DLod(TA, gl_TexCoord[0].st + vec2(-1.5/RES.x, 0.0), 3);
	gl_FragColor += texture2DLod(TA, gl_TexCoord[0].st + vec2(0.0, 1.5/RES.y), 3);
	gl_FragColor += texture2DLod(TA, gl_TexCoord[0].st + vec2(0.0, -1.5/RES.y), 3);
	gl_FragColor /= 5.0;
}