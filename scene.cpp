#include "scene.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "mesh.h"

#define CREATE_TEST_QUAD 0

Scene* _activeScene = 0;

Scene::Scene(QObject *parent): QObject(parent)
{
}

Scene* Scene::activeScene()
{
    if (_activeScene == 0) {
        _activeScene = new Scene(0);


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

        _activeScene->addMesh("quad", m);
#endif
    }

    return _activeScene;
}

void Scene::addMesh(QString name, Mesh *mesh)
{
    _meshes.insert(name, mesh);
    emit meshAdded();
}
