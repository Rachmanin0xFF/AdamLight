//VERTICAL//

#version 330 core

uniform sampler2D TA;
uniform vec2 RES;
uniform vec2 shadowResMult;
in vec2 texCoord;

void main(void) {
	gl_FragData[0].r = texture2D(TA, texCoord.st/shadowResMult).r;
	gl_FragData[0].r += texture2D(TA, (texCoord.st + vec2(0.0,  1.5/RES.y))/shadowResMult).r;
	gl_FragData[0].r += texture2D(TA, (texCoord.st + vec2(0.0, -1.5/RES.y))/shadowResMult).r;
	gl_FragData[0].r += texture2D(TA, (texCoord.st + vec2(0.0,  2.5/RES.y))/shadowResMult).r;
	gl_FragData[0].r += texture2D(TA, (texCoord.st + vec2(0.0, -2.5/RES.y))/shadowResMult).r;
	gl_FragData[0].r /= 5.0;
}