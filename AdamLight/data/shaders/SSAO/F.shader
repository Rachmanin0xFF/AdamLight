#version 120
uniform sampler2D TA;
uniform sampler2D TB;
uniform vec2 RES;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float getDepth(vec2 coords) {
	float zNear = 0.001;
	float zFar = 10000000.0;
    float z_b = texture2D(TA, coords).x;
    float z_n = 2.0*z_b - 1.0;
    return 1000.0*zNear*zFar/(zFar + zNear - z_n*(zFar - zNear));
}

float getMipMappy(vec2 coords, float LOD) {
	return texture2DLod(TA, coords, LOD).r;
}

vec3 rand3(vec3 p, vec3 q) {
	return (fract((p + q)*1750163.10121) - vec3(0.5, 0.5, 0.5));
}

void main(void) {
	float nominal_val = getDepth(gl_TexCoord[0].st);
	
	float sum = 0.0;
	float w = 0.0;
	vec2 sample_pos = vec2(0.0, 0.0);
	
	for(float i = 2.0; i < 50.0; i++) {
		vec2 blue = texture2D(TB, i*vec2(101.0, 139.0)/1024.0 + gl_FragCoord.xy/1024.0).rg-vec2(0.5, 0.5);
		sample_pos = gl_TexCoord[0].st + blue/RES.xy*200.0;
		
		float sample_val = getDepth(sample_pos);
		float contribution = 1.0;
		if(nominal_val - sample_val > 0.0) contribution = 0.0;
		
		float this_weight = 1.0 / (0.05 + length(blue));
		this_weight = 1.0;
		//if(length(blue) > 0.5) this_weight = 0.0;
		sum += contribution*this_weight;
		w += this_weight;
	}
	
	sum /= w;
	sum *= 2.0;
	gl_FragColor = vec4(sum, sum, sum, 1.0);
}