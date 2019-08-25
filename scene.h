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
    QHashIterator<QString,Mesh*> meshes() { return QHashIterator<QString,Mesh*>(_meshes); }
    void addMesh(QString name, Mesh* mesh);
signals:
    void meshAdded();
private:
    QHash<QString,Mesh*> _meshes;
};

#endif // SCENE_H
