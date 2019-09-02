#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "exporttexturesdialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDialog>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

#include "project.h"

#define RECENT_MESHES_SETTING "recent/meshes"
#define MAX_RECENT_PATHS 10

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QCoreApplication::setOrganizationName("Paint Bug");

    updateRecentMeshesMenu();

    connect(ui->importMeshAction, SIGNAL(triggered(bool)), this, SLOT(onImportMeshClicked(bool)));
    connect(ui->recentMeshMenu, SIGNAL(triggered(QAction*)), this, SLOT(onImportRecentMeshClicked(QAction*)));
    connect(ui->exportTexturesAction, SIGNAL(triggered(bool)), this, SLOT(onExportTexturesClicked(bool)));
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
    QString filePath = QFileDialog::getOpenFileName(this, "Import Mesh", "", "Meshes (*.obj)");
    if (!filePath.isNull()) {
        importMesh(filePath);
    }
}

void MainWindow::onImportRecentMeshClicked(QAction *event)
{
    QString filePath = event->text();
    importMesh(filePath);
}

void MainWindow::onExportTexturesClicked(bool c)
{
    ExportTexturesDialog dialog;
    dialog.exec();
}

void MainWindow::importMesh(QString filePath)
{
    std::cout << "importing mesh file: " << filePath << std::endl;

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filePath.toStdString(),
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

                m->setMeshName(mMesh->mName.C_Str());

                Project* project = Project::activeProject();
                project->addMesh(m);
            }
        }

        if (meshFailure) {
            QMessageBox::warning(this, tr("Error"),
                                 tr("At least one mesh was unable to be imported.  See log for more details"),
                                 QMessageBox::Ok);
        }

        QList<QString> recentMeshPaths = getRecentMeshPaths();

        int currentIndex = recentMeshPaths.indexOf(filePath);
        if (currentIndex > -1) {
            recentMeshPaths.removeAt(currentIndex);
        }
        recentMeshPaths.prepend(filePath);

        while (recentMeshPaths.count() > MAX_RECENT_PATHS) {
            recentMeshPaths.removeLast();
        }

        QSettings settings;
        settings.setValue(RECENT_MESHES_SETTING, QVariant(recentMeshPaths));
        settings.sync();
        updateRecentMeshesMenu();
    }
}

void MainWindow::showImportError()
{
    QMessageBox::warning(this, tr("Error"),
                         tr("Unable to import mesh file.  See log for more details"),
                         QMessageBox::Ok);

}

QList<QString> MainWindow::getRecentMeshPaths()
{
    QSettings settings;
    QList<QVariant> storedList = settings.value(RECENT_MESHES_SETTING).toList();
    QList<QString> list;
    foreach(QVariant v, storedList) {
        list << v.value<QString>();
    }
    return list;
}

void MainWindow::updateRecentMeshesMenu()
{
    ui->recentMeshMenu->clear();

    QList<QString> recentMeshPaths = getRecentMeshPaths();
    foreach (QString recentMeshPath, recentMeshPaths) {
        QFileInfo fileInfo(recentMeshPath);
        if (fileInfo.exists() && fileInfo.isFile()) {
            ui->recentMeshMenu->addAction(recentMeshPath);
        }
    }

    ui->recentMeshMenu->setEnabled(recentMeshPaths.count() > 0);
}
