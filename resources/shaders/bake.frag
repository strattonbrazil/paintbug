#version 330

uniform sampler2D meshTexture;
uniform sampler2D paintTexture;
uniform sampler2D drawTexture;
uniform vec4 brushColor;
uniform vec2 targetScale;
in vec2 meshUv;
in vec4 cameraPos;

bool isPainted(vec3 uv)
{
    float drawZ = texture2D(drawTexture, uv.xy).b; // looks between 0 and 1
    float meshZ = uv.z;

    return abs(drawZ - meshZ) < 0.0001;

    // TODO: disabled, but may be useful if depth testing isn't accurate enough
    //highp int screenPrimitiveId = int(texture2D(drawTexture, uv.xy).a);
    //return screenPrimitiveId == gl_PrimitiveID;
}

void main() {
    vec3 screenPos = 0.5 * (vec3(1,1,1) + cameraPos.xyz / cameraPos.w);
    vec3 paintUv = vec3(screenPos.xy * targetScale, screenPos.z);

    // get paint intensity and primitive id from screen coordinates
    float paintIntensity = 0.0f;
    if (isPainted(paintUv)) {
        paintIntensity = texture2D(paintTexture, paintUv.xy).r;
    }

    vec4 meshColor = texture2D(meshTexture, meshUv);
    vec3 diffuseColor = mix(meshColor.rgb, brushColor.rgb, paintIntensity);

    gl_FragColor = vec4(diffuseColor, 1);
}
