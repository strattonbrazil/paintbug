#include "exporttexturestablemodel.h"

#include "scene.h"

ExportTexturesTableModel::ExportTexturesTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    QVectorIterator<Mesh*> meshes = Scene::activeScene()->meshes();

    while (meshes.hasNext()) {
        Mesh* mesh = meshes.next();

        _bakeList.append(false);
        _meshNameList.append(mesh->meshName());
        _meshList.append(mesh);
    }
}

QVariant ExportTexturesTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        QVariant headers[] = {
            QVariant("Bake?"),
            QVariant("Mesh Name"),
            QVariant("Path")
        };
        return headers[section];
    }
    return QVariant("");
}

int ExportTexturesTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return _bakeList.count();
}

int ExportTexturesTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 3;
}

QVariant ExportTexturesTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::CheckStateRole && index.column() == 0) {
        if (_bakeList[index.row()])
            return Qt::Checked;
        else
            return Qt::Unchecked;
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == 1) {
            return _meshNameList[index.row()];
        } else if (index.column() == 2) {
            return _meshList[index.row()]->_texturePath;
        }
    }

    return QVariant();
}

bool ExportTexturesTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (role == Qt::CheckStateRole && index.column() == 0) {
        _bakeList[index.row()] = value.toBool();
        return true;
    } else if (index.column() == 2) {
        _meshList[index.row()]->_texturePath = value.toString();
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

Qt::ItemFlags ExportTexturesTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled;
    }

    if (index.column() == 0) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    } else if (index.column() == 1) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsEnabled;
    } else if (index.column() == 2) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsEnabled;;
    }

    return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled;
}

Mesh* ExportTexturesTableModel::mesh(const int meshIndex)
{
    return _meshList[meshIndex];
}
