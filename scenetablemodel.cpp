#include "scenetablemodel.h"
#include "project.h"

SceneTableModel::SceneTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    connect(Project::activeProject(), SIGNAL(meshAdded()), this, SLOT(onMeshAdded()));

    rebuildTable();
}

void SceneTableModel::onMeshAdded()
{
    _meshes.clear();
    rebuildTable();
}

void SceneTableModel::rebuildTable()
{
    QVectorIterator<Mesh*> meshes = Project::activeProject()->meshes();
    while (meshes.hasNext()) {
        Mesh *mesh = meshes.next();
        _meshes.append(mesh);
    }

    qSort(_meshes.begin(), _meshes.end(),
          []( Mesh *a,  Mesh *b) -> bool { return a->meshName() < b->meshName(); });

    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(_meshes.count() - 1, 1);

    endResetModel();
}

QVariant SceneTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        QVariant headers[] = {
            QVariant("Visible?"),
            QVariant("Mesh Name")
        };
        return headers[section];
    }

    return QVariant();
}

int SceneTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return _meshes.length();
}

int SceneTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 2;
}

QVariant SceneTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Project* project = Project::activeProject();
    Mesh *mesh = _meshes[index.row()];

    if (role == Qt::CheckStateRole && index.column() == 0) {
        if (project->meshVisible(mesh))
            return Qt::Checked;
        else
            return Qt::Unchecked;
    } else if (role == Qt::DisplayRole && index.column() == 1) {
        return _meshes[index.row()]->meshName();
    }

    return QVariant();
}

bool SceneTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    Project* project = Project::activeProject();

    if (role == Qt::CheckStateRole && index.column() == 0) {
        project->setMeshVisibility(_meshes[index.row()], value.toBool());
        emit dataChanged(index, index);
        return true;
    } else if (index.column() == 1) {
        _meshes[index.row()]->setMeshName(value.toString());
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

Qt::ItemFlags SceneTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled;
    }

    if (index.column() == 0) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    } else if (index.column() == 1) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsEnabled;
    }

    return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled;
}
