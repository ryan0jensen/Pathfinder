#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CustomScene *scene = new CustomScene;
    ui->graphicsView->setScene(scene);
    spaceType = 0;
    QBrush whiteBrush(Qt::white);
    QBrush greenBrush(Qt::green);
    QBrush redBrush(Qt::red);
    QPen blackPen(Qt::black);
    for (int x=0; x<=520; x+=20)
        scene->addLine(x,0,x,520, blackPen);
    for (int y=0; y<=520; y+=20)
        scene->addLine(0,y,520,y, blackPen);
    for(int i = 0; i<26; i++){
        for(int j = 0; j<26; j++){
            scene->mainGrid[i][j]=0;
        }
    }
    scene->startX=0, scene->startY=0;
    scene->endX=25, scene->endY=25;
    scene->mainGrid[0][0]=1;
    scene->mainGrid[25][25]=2;
    scene->addRect(0, 0, 20, 20, blackPen, greenBrush);
    scene->addRect(500, 500, 20, 20, blackPen, redBrush);
    scene->ui = this;
    connect(ui->selector, SIGNAL(activated(int)), this, SLOT(setSpaceType(int)));
    connect(ui->selector_2, SIGNAL(activated(int)), this, SLOT(setAlgoType(int)));
    connect(ui->pushButton, SIGNAL(clicked()), scene, SLOT(runPathfinder()));
    connect(ui->pushButton_2, SIGNAL(clicked()), scene, SLOT(updateGrid()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::spaceTypeSelected(){
    return this->spaceType;
}

int MainWindow::algoTypeSelected(){
    return this->algoType;
}

void MainWindow::setSpaceType(int index)
{
    this->spaceType = index;
    qDebug() << index;
    return;
}
void MainWindow::setAlgoType(int index)
{
    this->algoType = index;
    qDebug() << index;
    return;
}

void MainWindow::on_comboBox_activated(int index)
{
    qDebug() << index;
    return;
}

void CustomScene::updateGrid(){
    update();
    return;
}

void CustomScene::runPathfinder(){
    updateGrid(); // first clears the grid
    pathfinder(); // runs an algorithm according to what is selected
    return;
}
