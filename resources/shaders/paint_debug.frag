#version 120

varying vec3 worldPos;
varying vec2 uv;
uniform vec3 cameraPos;
uniform sampler2D meshTexture;
uniform sampler2D paintTexture;

void main() {
    float intensity = texture2D(paintTexture, uv).r;
    gl_FragColor = vec4(1, 0.2, 1, intensity);
}
