#version 120

uniform sampler2D TA; //depth
uniform sampler2D TB; //normal
uniform sampler2D TC; //color
uniform sampler2D TD; //specular

uniform vec2 RES;

uniform vec2 range;
uniform mat4 invProjMatrix;
uniform mat4 projMatrix;

vec3 rand3(vec3 p) {
	return (fract(p*1750163.10121) - vec3(0.5, 0.5, 0.5));
}

vec3 decodePosition(in float depth) {
	vec2 texCoord = gl_FragCoord.xy/RES;
	texCoord = texCoord * 2.0 - 1.0;
	vec4 projected = vec4(texCoord, depth, 1.0);
	vec4 unProjected = invProjMatrix * projected;
	unProjected.xyz /= unProjected.w;
	return unProjected.xyz * 0.5;
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

vec2 reproject(vec3 p) {
	vec4 v = vec4(vec3(p.xy, p.z), 1.0);
	v = projMatrix*v;
	v.xyz /= v.w;
	return v.xy*0.5 + 0.5;
}

float linDepth(float zval) {
    float z_b = zval;
    float z_n = 2.0 * z_b - 1.0;
    return range.x * range.y/(range.y + range.x - z_n * (range.y - range.x)) * 2.0;
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec4 ComputeReflection()
{
	//Tweakable variables
	float initialStepAmount = .01;
	float stepRefinementAmount = .7;
	int maxRefinements = 3;
	int maxDepth = 1;
	
	//Values from textures
	vec2 screenSpacePosition2D = gl_TexCoord[0].st;
	vec3 cameraSpacePosition = texture(positionTexture,screenSpacePosition2D).xyz;
	vec3 cameraSpaceNormal = texture(normalTexture,screenSpacePosition2D).xyz;
	float roughness = texture(otherTexture,screenSpacePosition2D).x;
	float reflectivity = texture(otherTexture,screenSpacePosition2D).y;
	
	//Screen space vector
	vec3 cameraSpaceViewDir = normalize(cameraSpacePosition);
	vec3 cameraSpaceVector = normalize(reflect(cameraSpaceViewDir,cameraSpaceNormal));
	vec3 screenSpacePosition = convertCameraSpaceToScreenSpace(cameraSpacePosition);
	vec3 cameraSpaceVectorPosition = cameraSpacePosition + cameraSpaceVector;
	vec3 screenSpaceVectorPosition = convertCameraSpaceToScreenSpace(cameraSpaceVectorPosition);
	vec3 screenSpaceVector = initialStepAmount*normalize(screenSpaceVectorPosition - screenSpacePosition);
	//Jitter the initial ray
	//float randomOffset1 = clamp(rand(gl_FragCoord.xy),0,1)/1000.0;
	//float randomOffset2 = clamp(rand(gl_FragCoord.yy),0,1)/1000.0;
	//screenSpaceVector += vec3(randomOffset1,randomOffset2,0);
	vec3 oldPosition = screenSpacePosition + screenSpaceVector;
	vec3 currentPosition = oldPosition + screenSpaceVector;
	//State
	vec4 color = vec4(0,0,0,1);
	int count = 0;
	int numRefinements = 0;
	int depth = 0;
	//Ray trace!
	while(depth < maxDepth) //doesnt do anything right now
	{
		while(count < 1000)
		{
			//Stop ray trace when it goes outside screen space
			if(currentPosition.x < 0 || currentPosition.x > 1 || currentPosition.y < 0 || currentPosition.y > 1 || currentPosition.z < 0 || currentPosition.z > 1)
				break;
			//intersections
			
			vec2 samplePos = currentPosition.xy;
			float currentDepth = linearizeDepth(currentPosition.z);
			float sampleDepth = linearizeDepth(texture(depthTexture, samplePos).x);
			float diff = currentDepth - sampleDepth;
			float error = length(screenSpaceVector);
			if(diff >= 0 && diff < error)
			{
				screenSpaceVector *= stepRefinementAmount;
				currentPosition = oldPosition;
				numRefinements++;
				if(numRefinements >= maxRefinements)
				{
					vec3 normalAtPos = texture(normalTexture, samplePos).xyz;
					float orientation = dot(cameraSpaceVector,normalAtPos);
					if(orientation < 0)
					{
						float cosAngIncidence = -dot(cameraSpaceViewDir,cameraSpaceNormal);
						cosAngIncidence = clamp(1-cosAngIncidence,0.0,1.0);
						color = texture(colorBufferTexture, samplePos) * cosAngIncidence;
					}
					break;
				}
			}
			//Step ray
			oldPosition = currentPosition;
			currentPosition = oldPosition + screenSpaceVector;
			count++;
		}
		depth++;
	}
	return color;
}

void main(void) {
	gl_FragColor = texture2D(TC, gl_TexCoord[0].st);
	vec4 samp = texture2D(TD, gl_TexCoord[0].st);
	if(samp.w > 0.15) {
		samp.xyz *= samp.w;
		vec3 color = samp.xyz;
		
		if(texture2D(TA, gl_TexCoord[0].st).r != 1.0 ) {
			float depth = texture2D(TA, gl_TexCoord[0].st).r;
			vec3 initial_position = decodePosition(depth);
			
			vec4 nsamp = texture2D(TB, gl_TexCoord[0].st);
			
			vec3 normal = decodeNormalInput(nsamp.rg);
			vec3 current_position = initial_position;
			vec3 final_position = current_position;
			vec3 reflected = normalize(reflect(normalize(initial_position), normalize(normal))) + rand3(initial_position)/nsamp.z*15.0;
			
			float stepSize = 0.4;
			float stepNum = 6;
			
			float range = stepSize*stepNum;
			
			for(float i = 0.0; i < 6; i++) {
			    current_position += reflected*stepSize;
				vec2 r2 = reproject(current_position);
				if(abs(-linDepth(texture2D(TA, r2).r) - current_position.z) < stepSize/1.3) {
					//gl_FragColor += texture2D(TC, r2)*(stepNum - i)/stepNum;//*vec4(255.0/255, 100.0/255, 10.0/255.0, 1.0);
					break;
				}
			}
			
			current_position -= stepSize*reflected;
			stepSize /= stepNum/2.6;
			range += stepSize*stepNum;
			for(float i = 0.0; i < 6; i++) {
			    current_position += reflected*stepSize;
				vec2 r2 = reproject(current_position);
				if(abs(-linDepth(texture2D(TA, r2).r) - current_position.z) < stepSize/1.3) {
					gl_FragColor += color*texture2D(TC, r2)*(range - length(current_position-initial_position))/range;//*vec4(255.0/255, 100.0/255, 10.0/255.0, 1.0);
					break;
				}
			}
		}
		gl_FragColor.r = pow(gl_FragColor.r, 1.5);
		gl_FragColor.g = pow(gl_FragColor.g, 1.5);
		gl_FragColor.b = pow(gl_FragColor.b, 1.5);
	}
}