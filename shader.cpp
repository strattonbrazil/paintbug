#include "shader.h"
#include "util.h"

#define VERSION_STRING "#version 120\n"

QGLShaderProgram* ShaderFactory::buildMeshShader(QObject *parent)
{
    QString vertSource(VERSION_STRING
                       "uniform mat4 objToWorld;\n"
                       "uniform mat4 cameraPV;\n"
                       "varying vec2 uv;\n"
                       "void main() {\n"
                       "  uv = gl_MultiTexCoord0.xy;\n"
                       "  gl_Position = cameraPV * objToWorld * gl_Vertex;\n"
                       "  gl_FrontColor = vec4(1,0,0,1);\n"
                       "}\n");

    QString fragSource(VERSION_STRING
                       "varying vec3 worldPos;\n"
                       "varying vec2 uv;\n"
                       "uniform vec3 cameraPos;\n"
                       "uniform vec4 brushColor;\n"
                       "uniform sampler2D meshTexture;\n"
                       "uniform sampler2D paintTexture;\n"
                       "uniform int paintFboWidth;\n"
                       "void main() {\n"
                       "    vec2 paintUvs = vec2(gl_FragCoord.x/paintFboWidth, gl_FragCoord.y/paintFboWidth); // faster to calc in vertex?\n"
                       "    float paintIntensity = texture2D(paintTexture, paintUvs).r;\n"
                       "    vec4 meshColor = texture2D(meshTexture, uv);\n"
                       "    vec3 diffuseColor = mix(meshColor.rgb, brushColor.rgb, paintIntensity);"
                       "    gl_FragColor = vec4(diffuseColor, 1);\n"
                       "    //gl_FragColor = meshColor;\n"
                       "}\n");

    QGLShader* vertShader = new QGLShader(QGLShader::Vertex);
    vertShader->compileSourceCode(vertSource);

    QGLShader* fragShader = new QGLShader(QGLShader::Fragment);
    fragShader->compileSourceCode(fragSource);

    QGLShaderProgram* program = new QGLShaderProgram(parent);
    program->addShader(vertShader);
    program->addShader(fragShader);

    return program;
}

QGLShaderProgram* ShaderFactory::buildBakeShader(QObject *parent)
{
    QString vertSource(VERSION_STRING
                       "uniform mat4 orthoPV;\n"
                       "uniform mat4 cameraPV;\n"
                       "uniform mat4 objToWorld;\n"
                       "varying vec2 meshUv;\n"
                       "varying vec4 cameraPos;\n"
                       "void main() {\n"
                       "  meshUv = gl_Vertex.xy;\n"
                       "  cameraPos = cameraPV * objToWorld * vec4(gl_MultiTexCoord0.xyz,1);\n"
                       "  //screenPos = 0.5 * (vec2(1,1) + (s.xy / s.w));\n"
                       "  //screenPos = gl_MultiTexCoord0.xy;\n"
                       "  gl_Position = orthoPV * gl_Vertex;\n"
                       "  gl_FrontColor = vec4(1,0,0,1);\n"
                       "}\n");

    QString fragSource(VERSION_STRING
                       "uniform sampler2D meshTexture;\n"
                       "uniform sampler2D paintTexture;\n"
                       "uniform vec4 brushColor;\n"
                       "varying vec2 meshUv;\n"
                       "varying vec4 cameraPos;\n"
                       "uniform vec2 targetScale;\n"
                       "void main() {\n"
                       "    vec2 screenPos = 0.5 * (vec2(1,1) + cameraPos.xy / cameraPos.w);\n"
                       "    vec2 paintUv = screenPos * targetScale;\n"
                       "    float paintIntensity = texture2D(paintTexture, paintUv).r;\n"
                       "    vec4 meshColor = texture2D(meshTexture, meshUv);\n"
                       "    vec3 diffuseColor = mix(meshColor.rgb, brushColor.rgb, paintIntensity);\n"
                       "    //if (screenPos.x > .5)\n"
                       "        //gl_FragColor = texture2D(paintTexture, paintUv);\n"
                       "    gl_FragColor = vec4(diffuseColor, 1);\n"
                       "}\n");

    QGLShader* vertShader = new QGLShader(QGLShader::Vertex);
    vertShader->compileSourceCode(vertSource);

    QGLShader* fragShader = new QGLShader(QGLShader::Fragment);
    fragShader->compileSourceCode(fragSource);

    QGLShaderProgram* program = new QGLShaderProgram(parent);
    program->addShader(vertShader);
    program->addShader(fragShader);

    return program;
}

QGLShaderProgram* ShaderFactory::buildPaintDebugShader(QObject *parent)
{
    QString vertSource(VERSION_STRING
                       "uniform mat4 cameraPV;\n" \
                       "varying vec2 uv;\n" \
                       "void main() {\n" \
                       "  uv = gl_MultiTexCoord0.xy;\n" \
                       "  gl_Position = cameraPV * gl_Vertex;\n" \
                       "}\n");

    QString fragSource(VERSION_STRING
                       "varying vec3 worldPos;\n"
                       "varying vec2 uv;\n"
                       "uniform vec3 cameraPos;\n"
                       "uniform sampler2D meshTexture;\n"
                       "uniform sampler2D paintTexture;\n"
                       "void main() {\n"
                       "    float intensity = texture2D(paintTexture, uv).r;\n"
                       "    gl_FragColor = vec4(1, 0.2, 1, intensity);\n"
                       "}\n");

    QGLShader* vertShader = new QGLShader(QGLShader::Vertex);
    vertShader->compileSourceCode(vertSource);

    QGLShader* fragShader = new QGLShader(QGLShader::Fragment);
    fragShader->compileSourceCode(fragSource);

    QGLShaderProgram* program = new QGLShaderProgram(parent);
    program->addShader(vertShader);
    program->addShader(fragShader);

    return program;
}
