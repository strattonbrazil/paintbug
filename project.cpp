#include "project.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "mesh.h"

#define CREATE_TEST_QUAD 0

Project* _activeProject = 0;

Project::Project(QObject *parent): QObject(parent)
{
}

Project* Project::activeProject()
{
    if (_activeProject == 0) {
        _activeProject = new Project(0);


#ifdef CREATE_TEST_QUAD
        Mesh* m = new Mesh();
        m->addVertex(-1, 0, 1);
        m->addVertex(1, 0, 1);
        m->addVertex(1, 0, -1);
        m->addVertex(-1, 0, -1);

        m->addUV(0,0);
        m->addUV(1,0);
        m->addUV(1,1);
        m->addUV(0,1);

        m->addTriangle(0, 1, 2);
        m->addTriangle(0, 2, 3);

        m->setMeshName("quad");

        _activeProject->addMesh(m);
#endif
    }

    return _activeProject;
}

void Project::addMesh(Mesh *mesh)
{
    _meshes.append(mesh);
    emit meshAdded();
}

void Project::reset()
{
    QList<Mesh*> removedMeshes = _meshes.toList();
    _meshes.clear();
    emit meshesRemoved(removedMeshes);
}

bool Project::meshVisible(Mesh *mesh)
{
    if (!_meshVisibilities.contains(mesh)) {
        _meshVisibilities[mesh] = true;
    }

    return _meshVisibilities[mesh];
}

void Project::setMeshVisibility(Mesh *mesh, bool visible)
{
    _meshVisibilities[mesh] = visible;
    QList<Mesh*> altered;
    altered.append(mesh);
    emit meshesAltered(altered);
}
