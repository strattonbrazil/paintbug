#version 330

uniform sampler2D meshTexture;
uniform sampler2D paintTexture;
uniform sampler2D drawTexture;
uniform vec4 brushColor;
uniform vec2 targetScale;
in vec2 meshUv;
in vec4 cameraPos;

bool isPainted(vec2 uv)
{
    highp int screenPrimitiveId = int(texture2D(drawTexture, uv).a);
    return screenPrimitiveId == gl_PrimitiveID;
}

void main() {
    vec2 screenPos = 0.5 * (vec2(1,1) + cameraPos.xy / cameraPos.w);
    vec2 paintUv = screenPos * targetScale;

    // get paint intensity and primitive id from screen coordinates
    float paintIntensity = 0.0f;
    if (isPainted(paintUv)) {
        paintIntensity = texture2D(paintTexture, paintUv).r;
    }

    vec4 meshColor = texture2D(meshTexture, meshUv);
    vec3 diffuseColor = mix(meshColor.rgb, brushColor.rgb, paintIntensity);
    //if (screenPos.x > .5)
        //gl_FragColor = texture2D(paintTexture, paintUv);
    gl_FragColor = vec4(diffuseColor, 1);
}
