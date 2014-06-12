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
                       "    //vec4 paintColor = brushColor * paintIntensity;\n"
                       "    vec4 meshColor = texture2D(meshTexture, uv);\n"
                       "    vec3 diffuseColor = mix(meshColor.rgb, brushColor.rgb, paintIntensity);"
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

QGLShaderProgram* ShaderFactory::buildBakeShader(QObject *parent)
{
    QString vertSource(VERSION_STRING
                       "uniform mat4 objToWorld;\n"
                       "uniform mat4 cameraPV;\n"
                       "varying vec2 uv;\n"
                       "void main() {\n"
                       "  uv = gl_Vertex.xy;\n"
                       "  gl_Position = cameraPV * objToWorld * gl_Vertex;\n"
                       "  gl_FrontColor = vec4(1,0,0,1);\n"
                       "}\n");

    QString fragSource(VERSION_STRING
                       "varying vec2 uv;\n"
                       "void main() {\n"
                       "    gl_FragColor = vec4(uv.x,uv.y,.1,1);\n"
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
