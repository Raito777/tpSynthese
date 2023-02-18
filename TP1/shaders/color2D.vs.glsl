#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec2 vFragPosition;
out vec3 vFragColor;


mat3 translate(float tx, float ty){
    return mat3(vec3(1, 0, 0), vec3(0, 1, 0), vec3(tx, ty, 1));
};

mat3 scale(float sx, float sy){
    return mat3(vec3(sx, 0, 0), vec3(0, sy, 0), vec3(0, 0, 1));
};

mat3 rotate(float a){
    float b = radians(a);
    return mat3(vec3(cos(b), sin(b), 0), vec3(-sin(b), cos(b), 0), vec3(0, 0, 1));
};


void main() {
  gl_Position = vec4((vec3(aVertexPosition, 1)*rotate(45)*translate(0.5,0)*scale(0.5,0.5)).xy, 0, 1);
  vFragColor = aVertexColor;
  vFragPosition = aVertexPosition;
};
