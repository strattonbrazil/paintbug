#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <QGLShaderProgram>

//using namespace std;

class ShaderFactory
{
public:
    static QGLShaderProgram* buildShader(QObject* parent, QString vertFile, QString fragFile);
    static QGLShaderProgram* buildMeshShader(QObject* parent = 0);
    static QGLShaderProgram* buildBakeShader(QObject* parent = 0);
    static QGLShaderProgram* buildPaintDebugShader(QObject* parent = 0);
};

#endif // SHADER_H
