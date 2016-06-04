#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDirModel>
#include <QMainWindow>
#include <QGraphicsView>
#include <QTreeWidgetItem>
#include "gxtfile.h"

using std::vector;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_splitter_splitterMoved(int pos, int index);

    void on_treeWidget_itemSelectionChanged();

    void on_actionSave_triggered();

private:
    Ui::MainWindow *ui;
    QVector<GxtFile*> gxtFiles;
    //QVector<CatFile*> catFiles;
    QGraphicsScene *scene;
    QByteArray *selectedImageData;
    void resizeEvent(QResizeEvent*);
    void scaleGraphicsView();
    void loadImageToView(int);
    int getSelectedItem();
    QString lastOpenedPath;
};

#endif // MAINWINDOW_H
