#version 330 core

in vec2 vFragPosition;
in vec4 centerPosition;
in vec3 vFragColor;

out vec3 fFragColor;

void main() {
  //vec2 fragment = vec2(gl_Position.x, gl_Position.y);
  float  distance = distance(centerPosition.xy, vFragPosition.xy);
  float a = 5*exp(-20*(distance*distance));
  fFragColor = vFragColor * a;
};