#version 330

in vec3 vColor;
in vec2 vTextureCoordinate;

out vec3 fFragColor;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;

void main() {
    vec3 color = vec3(texture(uTexture1, vTextureCoordinate))+vec3(texture(uTexture2, vTextureCoordinate));
    fFragColor = color;
}