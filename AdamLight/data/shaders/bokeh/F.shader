
uniform sampler2D TA;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
void main(void) {
	vec2 RES = vec2(1920, 1080);
	
	float w = 0.0;
	for(float f = 0.0; f < 30.0; f++) {
		float theta = 3.141593*f/15.0;
		float r = rand(vec2(f, 0.0) + gl_TexCoord[0].st);
		r = sqrt(r);
		vec2 offset = r*vec2(cos(theta), sin(theta))/RES*20.0;
		vec4 col = texture2D(TA, gl_TexCoord[0].st + offset);
		float w2 = col.r + col.g + col.b;
		gl_FragColor += col*w2;
		w+=w2;
	}
	gl_FragColor /= w;
}