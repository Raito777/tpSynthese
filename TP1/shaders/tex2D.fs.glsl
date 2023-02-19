#version 330

in vec3 vColor;
in vec2 aTextureCoordinate;

out vec3 fFragColor;

uniform sampler2D uTexture;

void main() {
    
    fFragColor = vec3(texture(uTexture, aTextureCoordinate));
}