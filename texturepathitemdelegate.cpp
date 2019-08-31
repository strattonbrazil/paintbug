#include "texturepathitemdelegate.h"

#include <QFileDialog>
#include <iostream>

TexturePathItemDelegate::TexturePathItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

QWidget* TexturePathItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QFileDialog *dialog = new QFileDialog(parent);
    dialog->setFileMode(QFileDialog::AnyFile);
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    return dialog;
}

void TexturePathItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QFileDialog *dialog = (QFileDialog*)editor;
    QStringList selected = dialog->selectedFiles();
    if (selected.count() > 0) {
        QString filePath = selected[0];
        model->setData(index, filePath);
    }
}
