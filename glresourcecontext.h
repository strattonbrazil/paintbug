#ifndef GLRESOURCECONTEXT_H
#define GLRESOURCECONTEXT_H

#include <QGLShaderProgram>

class GLResourceContext
{
public:
    GLResourceContext();
    QGLShaderProgram* meshShader();
    QGLShaderProgram* bakeShader();
    QGLShaderProgram* paintDebugShader();

private:
    QGLShaderProgram*         _meshShader;
    QGLShaderProgram*         _bakeShader;
    QGLShaderProgram*         _paintDebugShader;
};

#endif // GLRESOURCECONTEXT_H
