#ifndef SCENETABLEMODEL_H
#define SCENETABLEMODEL_H

#include <QAbstractTableModel>

#include "mesh.h"

class SceneTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SceneTableModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
public slots:
    void onMeshAdded();
private:
    void rebuildTable();

    QList<Mesh*> _meshes;
};

#endif // SCENETABLEMODEL_H
