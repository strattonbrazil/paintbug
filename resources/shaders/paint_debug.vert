#version 120

uniform mat4 cameraPV;
varying vec2 uv;

void main() {
  uv = gl_MultiTexCoord0.xy;
  gl_Position = cameraPV * gl_Vertex;
}
