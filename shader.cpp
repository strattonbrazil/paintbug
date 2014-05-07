#include "shader.h"
#include "util.h"

QGLShaderProgram* ShaderFactory::buildMeshShader(QObject *parent)
{
    QString vertSource("#version 120\n" \
                       "uniform mat4 objToWorld;\n" \
                       "uniform mat4 cameraPV;\n" \
                       "varying vec2 uv;\n" \
                       "void main() {\n" \
                       "  uv = gl_MultiTexCoord0.xy;\n" \
                       "  gl_Position = cameraPV * objToWorld * gl_Vertex;\n" \
                       "  gl_FrontColor = vec4(1,0,0,1);\n" \
                       "}\n");

    //std::cout << geomSource << std::endl;
    QString fragSource("#version 120\n" \
                       "varying vec3 worldPos;\n" \
                       "varying vec2 uv;\n" \
                       "uniform vec3 cameraPos;\n" \
                       "uniform sampler2D paintTexture;\n" \
                       "void main() {\n" \
                       "    // determine frag distance to closest edge\n" \
                       "    //float nearD = min(min(dist[0],dist[1]),dist[2]);\n" \
                       "    //float edgeIntensity = exp2(-edgeThickness*nearD*nearD);\n" \
                       "    //vec3 L = lightDir;\n" \
                       "    //vec3 V = normalize(cameraPos - worldPos);\n" \
                       "    //vec3 N = normalize(worldNormal);\n" \
                       "    //vec3 H = normalize(L+V);\n" \
                       "    //vec4 color = isSingleColor*singleColor + (1.0-isSingleColor)*gl_Color;\n" \
                       "    //float amb = 0.6;\n" \
                       "    //vec4 ambient = color * amb;\n" \
                       "    //vec4 diffuse = color * (1.0 - amb) * max(dot(L, N), 0.0);\n" \
                       "    //vec4 specular = vec4(0.0);\n" \
                       "    "
                       "    //float stippleMaskX = int(mod(gl_FragCoord.x / STIPPLE_SIZE, 2.0)) * int(mod((gl_FragCoord.x-STIPPLE_SIZE+1) / STIPPLE_SIZE, 2.0));\n" \
                       "    //float stippleMaskY = int(mod(gl_FragCoord.y / STIPPLE_SIZE, 2.0)) * int(mod((gl_FragCoord.y-STIPPLE_SIZE+1) / STIPPLE_SIZE, 2.0));\n" \
                       "    //vec4 combined = mix(vec4(ambient + diffuse + specular), vec4(0.3,0,0.3,1), stippleFaces * stippleMaskX * stippleMaskY);\n" \
                       "    //gl_FragData[0] = (edgeIntensity * vec4(0.1,0.1,0.1,1.0)) + ((1.0-edgeIntensity) * combined);\n" \
                       "    //gl_FragData[1] = selectIndex;\n" \
                       "    gl_FragColor = vec4(1,0,0,1);\n" \
                       "    gl_FragColor = vec4(uv.x, uv.y, 0, 1);\n" \
                       "    gl_FragColor = texture2D(paintTexture, uv.xy);\n" \
                       "}\n");

    //std::cout << vertSource.toStdString() << std::endl;
    //std::cout << geomSource.toStdString() << std::endl;
    //std::cout << fragSource.toStdString() << std::endl;

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

    //std::cout << geomSource << std::endl;
    QString fragSource("#version 120\n" \
                       "varying vec3 worldPos;\n" \
                       "varying vec2 uv;\n" \
                       "uniform vec3 cameraPos;\n" \
                       "uniform sampler2D paintTexture;\n" \
                       "void main() {\n" \
                       "    gl_FragColor = texture2D(paintTexture, uv.xy);\n" \
                       "}\n");

    //std::cout << vertSource.toStdString() << std::endl;
    //std::cout << geomSource.toStdString() << std::endl;
    //std::cout << fragSource.toStdString() << std::endl;

    QGLShader* vertShader = new QGLShader(QGLShader::Vertex);
    vertShader->compileSourceCode(vertSource);

    QGLShader* fragShader = new QGLShader(QGLShader::Fragment);
    fragShader->compileSourceCode(fragSource);

    QGLShaderProgram* program = new QGLShaderProgram(parent);
    program->addShader(vertShader);
    program->addShader(fragShader);

    return program;
}
