#version 120

varying vec3 worldPos;
varying vec2 uv;
uniform vec3 cameraPos;
uniform vec4 brushColor;
uniform sampler2D meshTexture;
uniform sampler2D paintTexture;
uniform int paintFboWidth;

void main() {
    vec2 paintUvs = vec2(gl_FragCoord.x/paintFboWidth, gl_FragCoord.y/paintFboWidth);
    float paintIntensity = texture2D(paintTexture, paintUvs).r;
    vec4 meshColor = texture2D(meshTexture, uv);
    vec3 diffuseColor = mix(meshColor.rgb, brushColor.rgb, paintIntensity);
    gl_FragColor = vec4(diffuseColor, 1);
    //gl_FragColor = vec4(gl_FragCoord.z, 0, 0, 1);
}
