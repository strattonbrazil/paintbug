#version 120

uniform sampler2D meshTexture;
uniform sampler2D paintTexture;
uniform vec4 brushColor;
varying vec2 meshUv;
varying vec4 cameraPos;
uniform vec2 targetScale;

void main() {
    vec2 screenPos = 0.5 * (vec2(1,1) + cameraPos.xy / cameraPos.w);
    vec2 paintUv = screenPos * targetScale;
    float paintIntensity = texture2D(paintTexture, paintUv).r;
    vec4 meshColor = texture2D(meshTexture, meshUv);
    vec3 diffuseColor = mix(meshColor.rgb, brushColor.rgb, paintIntensity);
    //if (screenPos.x > .5)
        //gl_FragColor = texture2D(paintTexture, paintUv);
    gl_FragColor = vec4(diffuseColor, 1);
}
