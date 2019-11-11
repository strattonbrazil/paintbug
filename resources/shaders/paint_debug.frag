#version 330

uniform vec3 cameraPos;
uniform sampler2D meshTexture;
uniform sampler2D paintTexture;
in vec3 worldPos;
in vec2 uv;

void main() {
    float intensity = texture2D(paintTexture, uv).r;
    gl_FragColor = vec4(1, 0.2, 1, intensity);
}
