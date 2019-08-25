#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

#include "scene.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->importMeshAction, SIGNAL(triggered(bool)), this, SLOT(onImportMeshClicked(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    // TODO: handle keys that bubble up
}

void MainWindow::onImportMeshClicked(bool c)
{
    QString fileName = QFileDialog::getOpenFileName(this, "Import Mesh", "", "Meshes (*.obj)");
    if (!fileName.isNull()) {
        std::cout << "importing file: " << fileName << std::endl;
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(fileName.toStdString(),
            aiProcess_CalcTangentSpace       |
            aiProcess_Triangulate            |
            aiProcess_JoinIdenticalVertices  |
            aiProcess_SortByPType);

        // If the import failed, report it
        if(!scene) {
            std::cerr << "error importing mesh: " << importer.GetErrorString() << std::endl;
            showImportError();
        } else if (!scene->HasMeshes()) {
            std::cerr << "error importing mesh: no meshes found" << std::endl;
            showImportError();
        } else {
            bool meshFailure = false;
            for (int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++) {
                aiMesh* mMesh = scene->mMeshes[meshIndex];
                if (!mMesh->HasPositions()) {
                    std::cerr << "error importing mesh (no positions detected): " << mMesh->mName.C_Str() << std::endl;
                    meshFailure = true;
                } else if (!mMesh->HasFaces()) {
                    std::cerr << "error importing mesh (no faces detected): " << mMesh->mName.C_Str() << std::endl;
                } else if (!mMesh->HasTextureCoords(0)) {
                    std::cerr << "error importing mesh (no texture coordinates detected): " << mMesh->mName.C_Str() << std::endl;
                    meshFailure = true;
                } else {
                    Mesh* m = new Mesh();
                    for (int vertIndex = 0; vertIndex < mMesh->mNumVertices; vertIndex++) {
                        aiVector3D v = mMesh->mVertices[vertIndex];
                        m->addVertex(v.x, v.y, v.z);
                    }

                    for (int uvIndex = 0; uvIndex < mMesh->mNumVertices; uvIndex++) {
                        aiVector3D uv = mMesh->mTextureCoords[0][uvIndex];
                        m->addUV(uv.x, uv.y);
                    }

                    for (int faceIndex = 0; faceIndex < mMesh->mNumFaces; faceIndex++) {
                        m->addTriangle(mMesh->mFaces[faceIndex].mIndices[0],
                                       mMesh->mFaces[faceIndex].mIndices[1],
                                       mMesh->mFaces[faceIndex].mIndices[2]);
                    }

                    Scene* scene = Scene::activeScene();
                    scene->addMesh(mMesh->mName.C_Str(), m);
                }
            }

            if (meshFailure) {
                QMessageBox::warning(this, tr("Error"),
                                     tr("At least one mesh was unable to be imported.  See log for more details"),
                                     QMessageBox::Ok);
            }
        }
    }
}

void MainWindow::showImportError()
{
    QMessageBox::warning(this, tr("Error"),
                         tr("Unable to import mesh file.  See log for more details"),
                         QMessageBox::Ok);

}
