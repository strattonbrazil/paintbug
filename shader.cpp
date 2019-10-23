#include "shader.h"
#include "util.h"

#define VERSION_STRING "#version 120\n"

QString resourceToString(QString resourcePath)
{
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cerr << "unable to read file path: " << resourcePath.toStdString() << std::endl;
        return QString();
    }

    return file.readAll();
}

QGLShaderProgram* shadersToProgram(QObject *parent, QString vertCode, QString fragCode)
{
    QGLShader* vertShader = new QGLShader(QGLShader::Vertex);
    vertShader->compileSourceCode(vertCode);

    QGLShader* fragShader = new QGLShader(QGLShader::Fragment);
    fragShader->compileSourceCode(fragCode);

    QGLShaderProgram* program = new QGLShaderProgram(parent);
    program->addShader(vertShader);
    program->addShader(fragShader);

    return program;
}

QGLShaderProgram* ShaderFactory::buildMeshShader(QObject *parent)
{
    return shadersToProgram(parent,
                            resourceToString(":/main/resources/shaders/mesh.vert"),
                            resourceToString(":/main/resources/shaders/mesh.frag"));
}

QGLShaderProgram* ShaderFactory::buildBakeShader(QObject *parent)
{
    return shadersToProgram(parent,
                            resourceToString(":/main/resources/shaders/bake.vert"),
                            resourceToString(":/main/resources/shaders/bake.frag"));
}

QGLShaderProgram* ShaderFactory::buildPaintDebugShader(QObject *parent)
{
    return shadersToProgram(parent,
                            resourceToString(":/main/resources/shaders/paint_debug.vert"),
                            resourceToString(":/main/resources/shaders/paint_debug.frag"));
}
