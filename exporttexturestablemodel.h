#ifndef EXPORTTEXTURESTABLEMODEL_H
#define EXPORTTEXTURESTABLEMODEL_H

#include <QAbstractTableModel>

#include "mesh.h"

class ExportTexturesTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ExportTexturesTableModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool exportTexture(const int meshIndex);
    Mesh* mesh(const int meshIndex);
private:    
    QVector<bool> _writeList;
    QVector<Mesh*> _meshList;

};

#endif // EXPORTTEXTURESTABLEMODEL_H
