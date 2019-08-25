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
private:
    void showImportError();
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
