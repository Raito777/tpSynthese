#version 330 core

in vec2 vFragPosition;
in vec3 vFragColor;

out vec3 fFragColor;

void main() {
  //vec2 fragment = vec2(gl_Position.x, gl_Position.y);
  float  distance = distance(gl_FragCoord.xy, vFragPosition.xy);
  float a = 1*exp(-1*(distance*distance));
  fFragColor = vFragColor * a;
};