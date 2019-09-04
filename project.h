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
signals:
    void meshAdded();
    void meshesRemoved(QList<Mesh*> removed);
private:
    QVector<Mesh*> _meshes;
};

#endif // PROJECT_H
