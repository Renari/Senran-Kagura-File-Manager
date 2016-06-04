#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();

    setWindowTitle("Senran Kagura File Manager");

    // I have no clue why this splitter stuff works this way.
    ui->splitter->setStretchFactor(0, 0);
    ui->splitter->setStretchFactor(1, 1);
    QList<int> sizes;
    sizes.append(ui->splitter->sizeHint().width() * .40);
    sizes.append(ui->splitter->sizeHint().width() * .60);
    ui->splitter->setSizes(sizes);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
}

void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(
                this,
                "Open File",
                lastOpenedPath.isEmpty() ? QDir::homePath() : lastOpenedPath,
                "Senran Kagura Resource File (*.gxt)");

    if (!filePath.isEmpty())
    {
        QFileInfo file = QFileInfo(filePath);
        lastOpenedPath = file.absoluteDir().absolutePath();

        ui->treeWidget->clear();

        gxtFiles.push_back(new GxtFile(filePath));

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, QFileInfo(filePath).baseName());

        int fileCount = gxtFiles.last()->getFileCount();

        for (int i = 0; i < fileCount; i++)
        {
            QTreeWidgetItem *childItem = new QTreeWidgetItem();
            childItem->setText(0, QString::number(i + 1));
            item->addChild(childItem);
        }
        ui->treeWidget->addTopLevelItem(item);
        ui->treeWidget->expandItem(item);
        ui->treeWidget->topLevelItem(0)->child(0)->setSelected(true);
    }
}

void MainWindow::on_splitter_splitterMoved(int pos, int index)
{
    scaleGraphicsView();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{

    QMainWindow::resizeEvent(event);
    scaleGraphicsView();
}

void MainWindow::scaleGraphicsView()
{
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::on_treeWidget_itemSelectionChanged()
{
    ui->actionSave->setEnabled(true);
    int selectedItem = getSelectedItem();
    if(selectedItem)
    {
        loadImageToView(selectedItem);
    }
    else
    {
        scene->clear();
    }
}

void MainWindow::loadImageToView(int index)
{
    scene->clear();
    QPixmap pixmap = gxtFiles.last()->readDDSFile(index);
    scene->setSceneRect(0, 0, pixmap.width(), pixmap.height());
    QGraphicsPixmapItem *texture =  new QGraphicsPixmapItem(pixmap);
    scene->addItem(texture);
    scaleGraphicsView();
}

void MainWindow::on_actionSave_triggered()
{
    int selectedItem = getSelectedItem();
    if (selectedItem)
    {
        QString path = QFileDialog::getSaveFileName(
                    this,
                    "Save File",
                    QDir::homePath(),
                    "DirectDraw Surface (*.dds)");
        QFile file(path);
        if(!file.open(QIODevice::WriteOnly))
            return;
        file.write(gxtFiles.last()->readFileData(getSelectedItem()));
        file.close();
    }
}

int MainWindow::getSelectedItem()
{
    QList<QTreeWidgetItem*> selectedItems = ui->treeWidget->selectedItems();
    if(selectedItems.size() > 0)
    {
        return ui->treeWidget->topLevelItem(0)->indexOfChild(selectedItems.first()) + 1;
    }
    else
    {
        return 0;
    }
}
