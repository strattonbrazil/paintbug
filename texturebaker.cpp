#include "texturebaker.h"

#include "texturecache.h"

TextureBaker::TextureBaker(QWidget *parent) : QOpenGLWidget(parent)
{

}

// returns true on successful save
bool TextureBaker::writeTextureToFile(Mesh *mesh)
{
    makeCurrent();

    if (!TextureCache::hasMeshTexture(mesh)) {
        return false;
    }

    GLuint textureId = TextureCache::meshTextureId(mesh);
    QString path = mesh->texturePath();
    const int size = TextureCache::meshTextureSize(mesh);
    std::cout << "baking texture id (" + QString::number(textureId) + ") to " << path.toStdString() << std::endl;

    const int NUM_COLOR_CHANNELS = 3;
    GLubyte data[size*size*NUM_COLOR_CHANNELS];

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    QImage out(data, size, size, QImage::Format_RGB888);
    out.mirrored().save(path);

    return true;
}

