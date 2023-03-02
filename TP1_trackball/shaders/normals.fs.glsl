#version 330 core

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTextureCoordinate;

out vec3 fFragColor;

void main() {
    fFragColor = normalize(vec3(vNormal));
};