#version 330 core

//SHADOW//

uniform sampler2D TA; //Albedo
uniform sampler2D TB; //Normals
uniform sampler2D TC; //Depth
uniform sampler2D TD; //Shadow
uniform sampler2D TE; //Specular information
uniform vec2 RES;
uniform vec2 SHADOWRES;
uniform vec2 shadowResMult;
uniform vec2 range;
uniform float time;

uniform vec3 camPos;

uniform vec3 lPos;
uniform float lRadius;
uniform float lIntensity;
uniform float shadowBlurs;
uniform vec3 lColor;
uniform vec3 shadowViewDir;

uniform float shadowZNear;
uniform float shadowZFar;
uniform mat4 shadowViewMatrix;

uniform mat4 viewMatrix;
uniform mat4 viewMatrixInverse;
uniform mat4 eyeViewToShadowViewProjMatrix;
uniform mat4 eyeViewToShadowViewMatrix;
uniform mat4 invProjMatrix;
uniform mat4 shadowViewProjMatrix;

out vec4 fragmentOut;

float rand(vec2 co) {
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233)))*43758.5453);
}

float linDepth(float zval) {
	float zNear = shadowZNear;
	float zFar = shadowZFar;
    float z_b = zval;
    float z_n = 2.0 * z_b - 1.0;
    return zNear * zFar/(zFar + zNear - z_n * (zFar - zNear)) * 2.0;
}

float linDepth2(float zval) {
	return zval * (shadowZFar - shadowZNear) + shadowZNear;
}

float linDepth3(float zval) {
	return -zval;
}

vec3 decodePosition(in float depth) {
	vec2 texCoord = gl_FragCoord.xy/RES;
	texCoord = texCoord * 2.0 - 1.0;
	vec4 projected = vec4(texCoord, depth, 1.0);
	vec4 unProjected = invProjMatrix * projected;
	unProjected.xyz /= unProjected.w;
	return unProjected.xyz * 0.5;
}

vec3 getShadowCoord(vec3 pos) {
	vec4 projected = eyeViewToShadowViewProjMatrix * vec4(pos, 1.0);
	projected.xy /= projected.w;
	projected.xy = projected.xy * 0.5 + 0.5;
	return projected.xyz;
}

vec3 decodeNormal(vec2 txc) {
	vec2 fnorm = texture2D(TB, txc).xy * 4.0 - 2.0;
	float f = dot(fnorm, fnorm);
	float g = sqrt(1.0 - f * 0.25);
	vec3 n = vec3(0.0, 0.0, 0.0);
	n.xy = fnorm * g;
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
	vec3 SC = getShadowCoord(position);
	
	if(SC.x < 1.0 && SC.y < 1.0 && SC.x > 0.0 && SC.y > 0.0 && SC.z > 0.0) {
		vec4 samp0 = texture2D(TA, texCoord);
		vec4 samp1 = texture2D(TB, texCoord);
		vec4 samp2 = texture2D(TE, texCoord);
		
		vec3 albedo = samp0.rgb;
		vec3 normal = decodeNormalInput(samp1.rg);
		vec3 lightPos = (viewMatrix * vec4(lPos, 1.0)).xyz;
		vec3 lightToPos = normalize(lightPos-position);
		vec3 eyeToPos = normalize(-position);
		
		float getTex = 0.0;
		if(shadowBlurs == 1) {
			float theta = rand(SC.xy)*314.2;
			vec2 sc = vec2(cos(theta), sin(theta))/SHADOWRES;
			getTex += texture2D(TD, (SC.xy + sc)/shadowResMult).r;
		} else
			getTex = texture2D(TD, SC.xy/shadowResMult).r;
		float projectedDistanceToLight = linDepth2(getTex);
		projectedDistanceToLight *= projectedDistanceToLight*0.0001 + 1.0;
		float distanceToLight = dot(shadowViewDir, position - lightPos);
		float shadowFalloff = clamp(exp(6.0 * (projectedDistanceToLight - distanceToLight)), 0.0, 1.0);
		
		float lightFalloff = max(0.0, lIntensity * (lRadius - length(position - lightPos))/lRadius) * (1.0 - length(SC.xy * 2.0-1.0));
		float falloff = lightFalloff * shadowFalloff;
		float diffuseIntensity = samp0.w * falloff * max(0.0, dot(normal, lightToPos));
		float specularIntensity = samp2.w * falloff * pow(clamp(dot(normal, normalize(lightToPos + eyeToPos)), 0.0, 1.0), samp1.z);
		
		fragmentOut = vec4(lColor * diffuseIntensity * albedo + specularIntensity * lColor * samp2.rgb, 1.0);
		
		fragmentOut.xyz = max(fragmentOut.xyz, 0.0);
	} else {
		fragmentOut = vec4(0.0, 0.0, 0.0, 1.0);
	}
}