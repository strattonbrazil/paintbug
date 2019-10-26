#version 120

uniform mat4 orthoPV;
uniform mat4 cameraPV;
uniform mat4 objToWorld;
attribute vec3 position;
attribute vec3 in_uvs;

varying vec2 meshUv;
varying vec4 cameraPos;

void main() {
  meshUv = position.xy;
  cameraPos = cameraPV * objToWorld * vec4(in_uvs,1);
  gl_Position = orthoPV * vec4(position, 1);;
}
