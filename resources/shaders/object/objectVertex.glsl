#version 410 core

const float DENSITY = 0.015;
const float GRADIANT = 1;
const int MAX_NUM_LIGHTS = 4;

in vec3 position;
in vec2 textureCoord;
in vec3 normal;

out vec2 passTextureCoord;
out vec3 surfaceNormal;
out vec3 toLightVectors[MAX_NUM_LIGHTS];
out vec3 toCameraVector;
out float visibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPositions[MAX_NUM_LIGHTS];
uniform int useFakeLighting;

uniform int textureAtlasSize;
uniform vec2 textureOffset;



void main() {
	vec4 worldPosition = model * vec4(position, 1.0);
	vec4 positionRelativeToCam = view * worldPosition;
	gl_Position = projection * positionRelativeToCam;
	passTextureCoord = (textureCoord / textureAtlasSize) + textureOffset;

	vec3 actualNormal = normal;
	if (useFakeLighting > 0.0) {
		actualNormal = vec3(0.0, 1.0, 0.0);
	}

	surfaceNormal = (model * vec4(actualNormal, 0.0)).xyz;
	
	for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
		toLightVectors[i] = lightPositions[i] - worldPosition.xyz;
	}
	
	toCameraVector = (inverse(view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;

	float distance = length(positionRelativeToCam.xyz);
	visibility = exp(-pow((distance * DENSITY), GRADIANT));
	visibility = clamp(visibility, 0.0, 1.0);
}
