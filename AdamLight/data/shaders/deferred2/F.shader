#version 330 core

//REGULAR//

uniform sampler2D TA;
uniform sampler2D TB;
uniform sampler2D TC;
uniform sampler2D TE;
uniform vec2 RES;
uniform vec2 range;

uniform vec3 camPos;

uniform vec3 lPos;
uniform float lRadius;
uniform float lIntensity;
uniform vec3 lColor;

uniform mat4 viewMatrix;
uniform mat4 invProjMatrix;

vec3 decodePosition(in float depth) {
	vec2 texCoord = gl_FragCoord.xy/RES;
	texCoord = texCoord*2.0 - 1.0;
	vec4 projected = vec4(texCoord, depth, 1.0);
	vec4 unProjected = invProjMatrix * projected;
	unProjected.xyz /= unProjected.w;
	return unProjected.xyz * 0.5;
}

vec3 decodeNormal(vec2 txc) {
	vec2 fnorm = texture2D(TB, txc).xy*4.0 - 2.0;
	float f = dot(fnorm, fnorm);
	float g = sqrt(1.0 - f/4);
	vec3 n = vec3(0.0, 0.0, 0.0);
	n.xy = fnorm*g;
	n.z = 1.0 - f/2.0;
	return n;
}

vec3 decodeNormalInput(vec2 val) {
	vec2 fnorm = val * 4.0 - 2.0;
	float f = dot(fnorm, fnorm);
	float g = sqrt(1.0 - f * 0.25);
	vec3 n = vec3(0.0, 0.0, 0.0);
	n.xy = fnorm * g;
	n.z = 1.0 - f/2.0;
	return n;
}

void main(void) {
	vec2 texCoord = gl_FragCoord.xy/RES;
	vec3 position = decodePosition(texture2D(TC, texCoord).r);
	
	vec4 samp0 = texture2D(TA, texCoord);
	vec4 samp1 = texture2D(TB, texCoord);
	vec4 samp2 = texture2D(TE, texCoord);
	
	vec3 albedo = samp0.rgb;
	vec3 normal = decodeNormalInput(samp1.rg);
	vec3 lightPos = (viewMatrix * vec4(lPos, 1.0)).xyz;
	vec3 lightToPos = normalize(lightPos-position);
	vec3 eyeToPos = normalize(-position);
	
	float falloff = max(0.0, lIntensity * (lRadius - length(position - lightPos))/lRadius);
	float diffuseIntensity = samp0.w * falloff * max(0.0, dot(normal, lightToPos));
	float specularIntensity = samp2.w * falloff * pow(clamp(dot(normal, normalize(lightToPos + eyeToPos)), 0.0, 1.0), samp1.z);
	
	gl_FragColor = vec4(lColor * diffuseIntensity * albedo + specularIntensity * lColor * samp2.rgb, 1.0);
	
	gl_FragColor.xyz = max(gl_FragColor.xyz, 0.0);
}