#include "scene.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "mesh.h"



Scene* _activeScene = 0;

Scene::Scene()
{
}

Scene* Scene::activeScene()
{
    if (_activeScene == 0) {
        _activeScene = new Scene();



        Mesh* m = new Mesh();
        m->addVertex(-1, 0, -1);
        m->addVertex(1, 0, -1);
        m->addVertex(1, 0, 1);
        m->addVertex(-1, 0, 1);

        m->addUV(0,0);
        m->addUV(1,0);
        m->addUV(1,1);
        m->addUV(0,1);

        m->addTriangle(0, 1, 2);
        m->addTriangle(0, 2, 3);

        /*
        Assimp::Importer importer;


        std::string pFile = "/home/stratton/models/capsule.obj";
        const aiScene* scene = importer.ReadFile( pFile,
              aiProcess_CalcTangentSpace       |
              aiProcess_Triangulate            |
              aiProcess_JoinIdenticalVertices  |
              aiProcess_SortByPType);

        for (int i = 0; i < scene->mNumMeshes; i++) {
            Mesh* m = new Mesh();

            aiMesh* mesh = scene->mMeshes[i];

            // add vertices
            for (int j = 0; j < mesh->mNumFaces; j++) {
                aiVector3D vertex = mesh->mVertices[j];
                m->addVertex(vertex.x, vertex.y, vertex.z);
            }

            // add faces
            for (int j = 0; j < mesh->mNumFaces; j++) {
                aiFace face = mesh->mFaces[j];

                m->addTriangle(face.mIndices[0],
                               face.mIndices[1],
                               face.mIndices[2]);
            }

            _activeScene->addMesh(mesh->mName.data, m);
       }
       */
        _activeScene->addMesh("quad", m);
    }

    return _activeScene;
}

void Scene::addMesh(QString name, Mesh *mesh)
{
    _meshes.insert(name, mesh);
}
