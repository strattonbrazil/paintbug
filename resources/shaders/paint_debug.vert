#version 330

uniform mat4 cameraPV;
out vec2 uv;

void main() {
  uv = gl_MultiTexCoord0.xy;
  gl_Position = cameraPV * gl_Vertex;
}
