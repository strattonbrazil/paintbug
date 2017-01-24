#include "glresourcecontext.h"

#include <QOpenGLFramebufferObject>

#include "shader.h"

GLResourceContext::GLResourceContext()
{
    _meshShader = 0;
    _bakeShader = 0;
    _paintDebugShader = 0;
    _paintFbo = 0;
}

QGLShaderProgram* GLResourceContext::meshShader()
{
    if (_meshShader == 0) {
        _meshShader = ShaderFactory::buildMeshShader();
    }
    return _meshShader;
}

QGLShaderProgram* GLResourceContext::bakeShader()
{
    if (_bakeShader == 0) {
        _bakeShader = ShaderFactory::buildBakeShader();
    }
    return _bakeShader;
}

QGLShaderProgram* GLResourceContext::paintDebugShader()
{

}

QOpenGLFramebufferObject* GLResourceContext::paintFbo()
{
    if (_paintFbo == 0) {
        QOpenGLFramebufferObjectFormat format;
        format.setInternalTextureFormat(GL_RED);
        _paintFbo = new QOpenGLFramebufferObject(PAINT_FBO_WIDTH, PAINT_FBO_WIDTH, format);

        _paintFbo->bind();
        glClearColor(0,0,0,0); // only red is used
        glClear(GL_COLOR_BUFFER_BIT);
        _paintFbo->release();
    }

    return _paintFbo;
}

QOpenGLFramebufferObject* GLResourceContext::transferFbo()
{
    if (!_transferFbo) {
        _transferFbo = new QOpenGLFramebufferObject(PAINT_FBO_WIDTH, PAINT_FBO_WIDTH);
    }
    return _transferFbo;
}
