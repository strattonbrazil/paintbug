#version 330

in vec2 uvs;
uniform vec3 cameraPos;
uniform vec4 brushColor;
uniform sampler2D meshTexture;
uniform sampler2D paintTexture;
uniform int paintFboWidth;

layout(location=0) out vec4 meshWithPaintColor;
layout(location=1) out vec4 primitiveId; // switch to int32 after new render target

void main() {
    vec2 paintUvs = vec2(gl_FragCoord.x/paintFboWidth, gl_FragCoord.y/paintFboWidth);
    float paintIntensity = texture(paintTexture, paintUvs).r;
    vec4 meshColor = texture(meshTexture, uvs);
    vec3 finalColor = mix(meshColor.rgb, brushColor.rgb, paintIntensity);

    meshWithPaintColor = vec4(finalColor, 0);

    // TODO: switch to single int32 after new render target
    primitiveId = vec4(1, 0, gl_FragCoord.z, gl_PrimitiveID + 0.5); // using .5 so I can safely round down
}
