#include "exporttexturesdialog.h"
#include "ui_exporttexturesdialog.h"

#include <QAbstractTableModel>

#include "scene.h"

ExportTexturesDialog::ExportTexturesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportTexturesDialog)
{
    ui->setupUi(this);

    _tableModel = new ExportTexturesTableModel(Scene::activeScene());
    _texturePathDelegate = new TexturePathItemDelegate(0);

    ui->texturesTable->setModel(_tableModel);
    ui->texturesTable->setItemDelegateForColumn(2, _texturePathDelegate);

    connect(ui->exportButton, SIGNAL(released()), this, SLOT(onExportClicked()));
    connect(ui->cancelButton, SIGNAL(released()), this, SLOT(close()));
}

ExportTexturesDialog::~ExportTexturesDialog()
{
    delete ui;
    delete _tableModel;
    delete _texturePathDelegate;
}

void ExportTexturesDialog::onExportClicked()
{
    for (int rowIndex = 0; rowIndex < _tableModel->rowCount(); rowIndex++) {
        //_tableModel->data(QModelIndex(row))
    }
}
