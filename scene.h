#ifndef SCENE_H
#define SCENE_H

#include <QHash>
#include "mesh.h"

class Scene : public QObject
{
    Q_OBJECT
public:
    Scene(QObject *parent);
    static Scene* activeScene();
    QVectorIterator<Mesh*> meshes() { return QVectorIterator<Mesh*>(_meshes); }
    void addMesh(Mesh* mesh);
signals:
    void meshAdded();
private:
    QVector<Mesh*> _meshes;
};

#endif // SCENE_H
