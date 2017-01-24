#ifndef GLRESOURCECONTEXT_H
#define GLRESOURCECONTEXT_H

#include <QGLShaderProgram>
#include <QOpenGLFramebufferObject>

#define PAINT_FBO_WIDTH 2048

class GLResourceContext
{
public:
    GLResourceContext();
    QGLShaderProgram* meshShader();
    QGLShaderProgram* bakeShader();
    QGLShaderProgram* paintDebugShader();

    QOpenGLFramebufferObject* paintFbo();
    QOpenGLFramebufferObject* transferFbo();
private:
    QGLShaderProgram*         _meshShader;
    QGLShaderProgram*         _bakeShader;
    QGLShaderProgram*         _paintDebugShader;

    QOpenGLFramebufferObject* _paintFbo;
    QOpenGLFramebufferObject* _transferFbo;
};

#endif // GLRESOURCECONTEXT_H
