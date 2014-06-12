#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <QGLShaderProgram>

//using namespace std;

class ShaderFactory
{
public:
    static QGLShaderProgram* buildShader(QObject* parent, QString vertFile, QString fragFile);
    static QGLShaderProgram* buildMeshShader(QObject* parent);
    static QGLShaderProgram* buildBakeShader(QObject* parent);
    static QGLShaderProgram* buildPaintDebugShader(QObject* parent);
};

#endif // SHADER_H
