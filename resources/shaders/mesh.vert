#version 330

uniform mat4 objToWorld;
uniform mat4 cameraPV;
in vec3 position;
in vec2 in_uvs;
out vec2 uvs;

void main() {
  uvs = in_uvs;
  gl_Position = cameraPV * objToWorld * vec4(position, 1);
}
