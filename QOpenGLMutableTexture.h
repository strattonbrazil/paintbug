#ifndef QOPENGLMUTABLETEXTURE_H
#define QOPENGLMUTABLETEXTURE_H

#include <QOpenGLTexture>

class QOpenGLMutableTexture : public QOpenGLTexture
{
public:
    QOpenGLMutableTexture(QImage img) : QOpenGLTexture(img) {}
    //bool isUsingImmutableStorage() { return true; }
//    QOpenGLTexture::PixelFormat pixelFormat() {
//        const QOpenGLTexturePrivate *d = this->d_ptr.data();
//        //QOpenGLTexturePrivate *texP = d_ptr.pointer;
//    }
private:
    //const QOpenGLTexture::PixelType;
};

#endif // QOPENGLMUTABLETEXTURE_H
