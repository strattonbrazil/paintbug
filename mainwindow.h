#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyReleaseEvent(QKeyEvent *event);
public slots:
    void onImportMeshClicked(bool c);
    void onImportRecentMeshClicked(QAction *event);
private:
    void importMesh(QString filePath);
    void showImportError();
    QList<QString> getRecentMeshPaths();
    void updateRecentMeshesMenu();
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
