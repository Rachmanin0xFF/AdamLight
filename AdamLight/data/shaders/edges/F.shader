#version 120
uniform sampler2D TA;
uniform sampler2D TB;
void main(void) {
	float k = 1.0/1024.0;
	vec3 q = texture2D(TA, gl_TexCoord[0].st).rgb;
	vec3 q1 = texture2D(TA, gl_TexCoord[0].st + vec2(k, 0.0)).rgb;
	vec3 q2 = texture2D(TA, gl_TexCoord[0].st + vec2(-k, 0.0)).rgb;
	vec3 q3 = texture2D(TA, gl_TexCoord[0].st + vec2(0.0, k)).rgb;
	vec3 q4 = texture2D(TA, gl_TexCoord[0].st + vec2(0.0, -k)).rgb;
	vec3 b = abs(q - (q1+q2+q3+q4)/4.0);
	gl_FragData[0].rgb = b*4.0;
}