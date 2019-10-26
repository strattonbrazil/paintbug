#version 120

uniform mat4 objToWorld;
uniform mat4 cameraPV;
attribute vec3 position;
attribute vec2 in_uvs;
varying vec2 uvs;

void main() {
  uvs = in_uvs;
  gl_Position = cameraPV * objToWorld * vec4(position, 1);
}
