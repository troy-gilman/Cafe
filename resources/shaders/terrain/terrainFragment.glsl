#version 410 core

const int MAX_NUM_LIGHTS = 4;

in vec2 passTextureCoord;
in vec3 surfaceNormal;
in vec3 toLightVectors[MAX_NUM_LIGHTS];
in vec3 toCameraVector;
in float visibility;

out vec4 outColor;

uniform sampler2D blendMap;
uniform sampler2D backgroundTexture;
uniform sampler2D rTexture;
uniform sampler2D gTexture;
uniform sampler2D bTexture;


uniform sampler2D tex;
uniform vec3 lightColors[MAX_NUM_LIGHTS];
uniform vec3 lightAttenuations[MAX_NUM_LIGHTS];
uniform int numLights;
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 skyColor;

void main() {

	vec4 blendMapColor = texture(blendMap, passTextureCoord);

	float backTextureAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
	vec2 tiledCoords = passTextureCoord * 40.0;
	vec4 backgroundTextureColor = texture(backgroundTexture, tiledCoords) * backTextureAmount;
	vec4 rTextureColor = texture(rTexture, tiledCoords) * blendMapColor.r;
	vec4 gTextureColor = texture(gTexture, tiledCoords) * blendMapColor.g;
	vec4 bTextureColor = texture(bTexture, tiledCoords) * blendMapColor.b;

	vec4 totalColor = backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;


	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitVectorToCamera = normalize(toCameraVector);

	vec3 totalDiffuse = vec3(0.0, 0.0, 0.0);
	vec3 totalSpecular = vec3(0.0, 0.0, 0.0);

	for (int i = 0; i < numLights; i++) {
		vec3 toLightVector = toLightVectors[i];
		vec3 lightColor = lightColors[i];
		vec3 lightAttenuation = lightAttenuations[i];
		float distanceFromLight = length(toLightVector);
		float attenuationFactor = lightAttenuation.x + (lightAttenuation.y * distanceFromLight) + (lightAttenuation.z * distanceFromLight * distanceFromLight);
		vec3 unitLightVector = normalize(toLightVector);
		float nDotl = dot(unitNormal, unitLightVector);
		float brightness = max(nDotl, 0.0);
		vec3 lightDirection = -unitLightVector;
		vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
		float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
		specularFactor = max(specularFactor, 0.0);
		float dampedFactor = pow(specularFactor, shineDamper);
		vec3 diffuse = (brightness * lightColor) / attenuationFactor;
		vec3 specular = (dampedFactor * reflectivity * lightColor) / attenuationFactor;
		totalDiffuse = totalDiffuse + diffuse;
		totalSpecular = totalSpecular + specular;
	}
	totalDiffuse = max(totalDiffuse, 0.1);

	outColor = vec4(totalDiffuse, 1.0) * totalColor + vec4(totalSpecular, 1.0);
	outColor = mix(vec4(skyColor, 1.0), outColor, visibility);
}
