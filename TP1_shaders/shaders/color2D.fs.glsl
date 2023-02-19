#version 330 core

in vec2 vFragPosition;
in vec4 centerPosition;
in vec3 vFragColor;

out vec3 fFragColor;

void main() {
  vec2 P = vFragPosition.xy;
  float distance = distance(centerPosition.xy, P);
  float a = 1.5*exp(-10*(distance*distance));
  fFragColor = vFragColor * length(fract(5.0 * P));
  fFragColor = vFragColor * length(abs(fract(5.0 * P) * 2.0 - 1.0));
  fFragColor = vFragColor * mod(floor(10.0 * P.x) + floor(10.0 * P.y), 2.0);
  fFragColor = vFragColor * smoothstep(0.3, 0.32, length(fract(5.0 * P) - 0.5));
  fFragColor = vFragColor * smoothstep(0.4, 0.5, max(abs(fract(8.0 * P.x - 0.5 * mod(floor(8.0 * P.y), 2.0)) - 0.5), abs(fract(8.0 * P.y) - 0.5)));

};