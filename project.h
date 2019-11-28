#ifndef PROJECT_H
#define PROJECT_H

#include <QHash>
#include "mesh.h"

class Project : public QObject
{
    Q_OBJECT
public:
    Project(QObject *parent);
    static Project* activeProject();
    QVectorIterator<Mesh*> meshes() { return QVectorIterator<Mesh*>(_meshes); }
    void addMesh(Mesh* mesh);
    void reset();
    bool meshVisible(Mesh *mesh);
    void setMeshVisibility(Mesh *mesh, bool visible);
signals:
    void meshAdded();
    void meshesRemoved(QList<Mesh*> removed);
    void meshesAltered(QList<Mesh*> altered);
private:
    QVector<Mesh*> _meshes;
    // TODO: probably combine with _meshes as hash of Mesh to project properties
    QHash<Mesh*,bool> _meshVisibilities;
};

#endif // PROJECT_H
