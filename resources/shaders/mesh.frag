#version 120


varying vec2 uvs;
uniform vec3 cameraPos;
uniform vec4 brushColor;
uniform sampler2D meshTexture;
uniform sampler2D paintTexture;
uniform int paintFboWidth;

void main() {
    vec2 paintUvs = vec2(gl_FragCoord.x/paintFboWidth, gl_FragCoord.y/paintFboWidth);
    float paintIntensity = texture2D(paintTexture, paintUvs).r;
    vec4 meshColor = texture2D(meshTexture, uvs);
    vec3 diffuseColor = mix(meshColor.rgb, brushColor.rgb, paintIntensity);

    // note: putting depth in alpha channel for convenience, may move to another
    //       color attachment in the future if alpha needed
    gl_FragColor = vec4(diffuseColor, gl_FragCoord.z);
}
