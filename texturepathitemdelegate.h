#ifndef TEXTUREPATHITEMDELEGATE_H
#define TEXTUREPATHITEMDELEGATE_H

#include <QStyledItemDelegate>

class TexturePathItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TexturePathItemDelegate(QObject *parent = nullptr);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
signals:

public slots:
};

#endif // TEXTUREPATHITEMDELEGATE_H
