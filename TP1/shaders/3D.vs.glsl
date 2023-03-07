#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

//Variables de sorties
out vec3 vPosition_vs;
out vec3 vNormal_vs;
out vec2 vTexCoords;

void main() {
    //Passage en coordonnées homogènes
    vec4 vertexPosition = vec4(aPosition, 1);
    vec4 vertexNormal = vec4(aNormal, 1);
    
    //Calcul des variables de sorties
    vPosition_vs = vec3(uMVMatrix * vertexPosition);
    vNormal_vs = vec3(uNormalMatrix * vertexNormal);
    vTexCoords = aTexCoords;
    
    gl_Position = uMVPMatrix * vertexPosition;
};
