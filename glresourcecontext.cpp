#include "glresourcecontext.h"

#include "shader.h"

GLResourceContext::GLResourceContext()
{
    _meshShader = 0;
    _bakeShader = 0;
    _paintDebugShader = 0;
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

}

QGLShaderProgram* GLResourceContext::paintDebugShader()
{

}
