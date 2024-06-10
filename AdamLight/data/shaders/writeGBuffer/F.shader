#version 330 core

uniform sampler2D TA;
uniform sampler2D TB;
uniform sampler2D TC;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
in vec2 texCoord;

uniform float albedo_texture_on;
uniform float normal_texture_on;
uniform float specular_intensity;
uniform vec3 specular_color;
uniform float hardness;
uniform float diffuse_intensity;
uniform vec3 diffuse_color;
uniform float ambient_color;

//haha crysis 3 i stole ur normal buffer encoding kind of
vec2 encodeNormal(vec3 n) {
    float f = sqrt(8*n.z + 8.0);
    return n.xy/f + 0.5;
}

void main(void) {
	vec3 n = normalize(normal);
	
	if(normal_texture_on > 0.5) {
		vec2 tn = texture2D(TC, texCoord).xy;
		if(tn != vec2(0.0, 0.0)) {
			vec3 N0 = normalize(normal);
			vec3 T0 = normalize(tangent);
			vec3 B0 = normalize(bitangent);
			
			vec3 nmap;
			nmap.xy = ((2.0*tn) - 1.0)*0.5;
			nmap.xy = nmap.xy;
			nmap.z = sqrt(1.0 - dot(nmap.xy, nmap.xy));
			N0 = normalize((T0*nmap.x) + (B0*nmap.y) + (N0*nmap.z));
			n = N0;
		}
	}
	
	vec3 c = diffuse_color;
	
	if(albedo_texture_on > 0.5) {
		c *= texture2D(TA, texCoord).rgb;
	}
	gl_FragData[0] = vec4(c, diffuse_intensity);
	gl_FragData[1] = vec4(encodeNormal(n), hardness, 0.0);
	gl_FragData[2].xyz = c*ambient_color;
	gl_FragData[3] = vec4(specular_color, specular_intensity);
}