#version 330

uniform mat4 orthoPV;
uniform mat4 cameraPV;
uniform mat4 objToWorld;
in vec3 position;
in vec3 in_uvs;
out vec2 meshUv;
out vec4 cameraPos;

void main() {
  meshUv = position.xy;
  cameraPos = cameraPV * objToWorld * vec4(in_uvs,1);
  gl_Position = orthoPV * vec4(position, 1);;
}
