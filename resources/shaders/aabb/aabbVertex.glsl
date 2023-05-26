#version 410 core

in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 worldPosition = model * vec4(position, 1.0);
    vec4 positionRelativeToCam = view * worldPosition;
    gl_Position = projection * positionRelativeToCam;
}