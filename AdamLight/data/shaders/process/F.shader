uniform sampler2D TA;

float sat(float t) {
	return clamp(t, 0.0, 1.0);
}
float linterp(float t) {
	return sat(1.0 - abs(2.0 * t - 1.0 ));
}
float remap(float t, float a, float b) {
	return sat((t - a) / (b - a));
}
vec3 spectrum_offset( float t ) {
	vec3 ret;
	float lo = step(t,0.5);
	float hi = 1.0-lo;
	float w = linterp( remap( t, 1.0/6.0, 5.0/6.0 ) );
	ret = vec3(lo,1.0,hi) * vec3(1.0-w, w, 1.0-w);
	return pow( ret, vec3(1.0/2.2) );
}

void main(void) {
	vec3 s = texture2D(TA, gl_TexCoord[0].st).rgb;
	float b = (s.r + s.g + s.b)/3.0;
	float b2 = length(gl_TexCoord[0].st - 0.5);
	b2 *= b2*b2;
	gl_FragColor.rgb = s*(1.0-b2);
}