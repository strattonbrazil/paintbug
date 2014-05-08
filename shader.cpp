#include "shader.h"
#include "util.h"

QGLShaderProgram* ShaderFactory::buildMeshShader(QObject *parent)
{
    QString vertSource("#version 120\n"
                       "uniform mat4 objToWorld;\n"
                       "uniform mat4 cameraPV;\n"
                       "varying vec2 uv;\n"
                       "void main() {\n"
                       "  uv = gl_MultiTexCoord0.xy;\n"
                       "  gl_Position = cameraPV * objToWorld * gl_Vertex;\n"
                       "  gl_FrontColor = vec4(1,0,0,1);\n"
                       "}\n");

    QString fragSource("#version 120\n"
                       "varying vec3 worldPos;\n"
                       "varying vec2 uv;\n"
                       "uniform vec3 cameraPos;\n"
                       "uniform sampler2D paintTexture;\n"
                       "uniform int paintFboWidth;\n"
                       "void main() {\n"
                       "    vec2 paintUvs = vec2(gl_FragCoord.x/paintFboWidth, gl_FragCoord.y/paintFboWidth); // faster to calc in vertex?\n"
                       "    //gl_FragColor = vec4(uv.x, uv.y, 0, 1);\n"
                       "    gl_FragColor = texture2D(paintTexture, paintUvs);\n"
                       "    //if (paintUvs.t > 0.5) gl_FragColor = vec4(1,0,1,1);\n"
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
    QString vertSource("#version 120\n" \
                       "uniform mat4 cameraPV;\n" \
                       "varying vec2 uv;\n" \
                       "void main() {\n" \
                       "  uv = gl_MultiTexCoord0.xy;\n" \
                       "  gl_Position = cameraPV * gl_Vertex;\n" \
                       "}\n");

    QString fragSource("#version 120\n"
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
