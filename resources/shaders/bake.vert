#version 120

uniform mat4 orthoPV;
uniform mat4 cameraPV;
uniform mat4 objToWorld;
varying vec2 meshUv;
varying vec4 cameraPos;

void main() {
  meshUv = gl_Vertex.xy;
  cameraPos = cameraPV * objToWorld * vec4(gl_MultiTexCoord0.xyz,1);
  gl_Position = orthoPV * gl_Vertex;
}
