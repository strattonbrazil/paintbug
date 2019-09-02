#include "exporttexturesdialog.h"
#include "ui_exporttexturesdialog.h"

#include <QAbstractTableModel>
#include <QProgressDialog>

#include "project.h"

ExportTexturesDialog::ExportTexturesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportTexturesDialog)
{
    ui->setupUi(this);

    _tableModel = new ExportTexturesTableModel(Project::activeProject());
    _texturePathDelegate = new TexturePathItemDelegate(0);

    ui->texturesTable->setModel(_tableModel);
    ui->texturesTable->setItemDelegateForColumn(2, _texturePathDelegate);

    connect(_tableModel, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(onDataChanged(QModelIndex,QModelIndex,QVector<int>)));
    connect(ui->exportButton, SIGNAL(released()), this, SLOT(onExportClicked()));
    connect(ui->cancelButton, SIGNAL(released()), this, SLOT(close()));
}

ExportTexturesDialog::~ExportTexturesDialog()
{
    delete ui;
    delete _tableModel;
    delete _texturePathDelegate;
}

void ExportTexturesDialog::onDataChanged(QModelIndex index1, QModelIndex index2, QVector<int> roles)
{
    if (index1.column() == 0) {
        bool exportEnabled = false;
        for (int rowIndex = 0; rowIndex < _tableModel->rowCount(); rowIndex++) {
            if (_tableModel->exportTexture(rowIndex)) {
                exportEnabled = true;
                break;
            }
        }
        ui->exportButton->setEnabled(exportEnabled);
    }
}

void ExportTexturesDialog::onExportClicked()
{
    ui->progressBar->setEnabled(true);
    for (int rowIndex = 0; rowIndex < _tableModel->rowCount(); rowIndex++) {
        if (_tableModel->exportTexture(rowIndex)) {
            Mesh *mesh = _tableModel->mesh(rowIndex);
            ui->textureBaker->writeTextureToFile(mesh);
        }
        ui->progressBar->setValue(rowIndex * 100 / _tableModel->rowCount());
    }
    ui->progressBar->setValue(100);
}
