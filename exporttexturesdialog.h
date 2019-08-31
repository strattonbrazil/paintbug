#ifndef EXPORTTEXTURESDIALOG_H
#define EXPORTTEXTURESDIALOG_H

#include <QDialog>

#include "exporttexturestablemodel.h"
#include "texturepathitemdelegate.h"

namespace Ui {
class ExportTexturesDialog;
}

class ExportTexturesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportTexturesDialog(QWidget *parent = 0);
    ~ExportTexturesDialog();
public slots:
    void onExportClicked();
private:
    Ui::ExportTexturesDialog *ui;
    ExportTexturesTableModel *_tableModel;
    TexturePathItemDelegate *_texturePathDelegate;
};

#endif // EXPORTTEXTURESDIALOG_H
