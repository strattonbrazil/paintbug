#ifndef SCENE_H
#define SCENE_H

#include <QHash>
#include "mesh.h"

class Scene
{
public:
    Scene();
    static Scene* activeScene();
    QHashIterator<QString,Mesh*> meshes() { return QHashIterator<QString,Mesh*>(_meshes); }
private:
    void addMesh(QString name, Mesh* mesh);
    QHash<QString,Mesh*> _meshes;
};

#endif // SCENE_H
