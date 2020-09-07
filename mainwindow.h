#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QtCore>
#include <QtGui>
#include <QComboBox>
#include <QPushButton>
#include <queue>
#include <utility>
#include <functional>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int spaceType;
    int algoType;
    int spaceTypeSelected();
    int algoTypeSelected();
public slots:
    void on_comboBox_activated(int index);
    void setSpaceType(int index);
    void setAlgoType(int index);

private:
    Ui::MainWindow *ui;
    QGraphicsRectItem *rect;
    QComboBox *selector;
    QComboBox *selector_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
};

class CustomScene : public QGraphicsScene
{
    Q_OBJECT
public:
    int mainGrid[26][26];
    int startX;
    int startY;
    int endX;
    int endY;
    MainWindow *ui;
public slots:
    void runPathfinder();
    void updateGrid();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        qDebug() << "test";
        QPointF mousePos = event->scenePos();
        qDebug() << mousePos;
        qreal xpos = mousePos.x();
        qreal ypos = mousePos.y();
        int x = 0;
        int y = 0;
        while(xpos>0){
            x++;
            xpos-=20;
        }
        while(ypos>0){
            y++;
            ypos-=20;
        }
        qDebug() << x;
        qDebug() << y;
        QBrush greenBrush(Qt::green);
        QBrush redBrush(Qt::red);
        QBrush whiteBrush(Qt::white);
        QBrush grayBrush(Qt::gray);
        QPen outlinePen(Qt::black);
        int index = ui->spaceTypeSelected(); // index==0 is space, 1 is starting point, 2 is end point, 3 is wall
        if(index==0){
            if(mainGrid[x-1][y-1]!=1 && mainGrid[x-1][y-1]!=2){
                this->addRect(20*(x-1), 20*(y-1), 20, 20, outlinePen, whiteBrush);
                mainGrid[x-1][y-1]=0;
            }
        } else if (index==1){
            if(mainGrid[x-1][y-1]!=1 && mainGrid[x-1][y-1]!=2){
                this->addRect(20*(x-1), 20*(y-1), 20, 20, outlinePen, greenBrush);
                this->addRect(startX*20, startY*20, 20, 20, outlinePen, whiteBrush);
                mainGrid[x-1][y-1]=1;
                mainGrid[startX][startY]=0;
                startX=x-1, startY=y-1;
            }
        } else if (index==2){
            if(mainGrid[x-1][y-1]!=1 && mainGrid[x-1][y-1]!=2){
                this->addRect(20*(x-1), 20*(y-1), 20, 20, outlinePen, redBrush);
                this->addRect(endX*20, endY*20, 20, 20, outlinePen, whiteBrush);
                mainGrid[x-1][y-1]=2;
                mainGrid[endX][endY]=0;
                endX=x-1, endY=y-1;
            }
        } else if (index==3){
            if(mainGrid[x-1][y-1]!=1 && mainGrid[x-1][y-1]!=2){
                this->addRect(20*(x-1), 20*(y-1), 20, 20, outlinePen, grayBrush);
                mainGrid[x-1][y-1]=3;
            }
        }
    }
private:
    void update(){
        QBrush greenBrush(Qt::green);
        QBrush redBrush(Qt::red);
        QBrush whiteBrush(Qt::white);
        QBrush grayBrush(Qt::gray);
        QPen outlinePen(Qt::black);
        for(int i = 0; i<26; i++){
            for(int j = 0; j<26; j++){
                if(mainGrid[i][j]==0){
                    this->addRect(20*i, 20*j, 20, 20, outlinePen, whiteBrush);
                } else if(mainGrid[i][j]==1){
                    this->addRect(20*i, 20*j, 20, 20, outlinePen, greenBrush);
                } else if(mainGrid[i][j]==2){
                    this->addRect(20*i, 20*j, 20, 20, outlinePen, redBrush);
                } else if(mainGrid[i][j]==3){
                    this->addRect(20*i, 20*j, 20, 20, outlinePen, grayBrush);
                }
            }
        }
    }
    void delay(int ms)
    {
        QTime dieTime= QTime::currentTime().addMSecs(ms);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    bool valid(int x, int y){
        if(x<0 || y<0 || x>25 || y>25) return false;
        return true;
    }
    int distance(int x, int y){
        int xDist = x-endX;
        if(xDist<0) xDist = xDist*-1;
        int yDist = y-endY;
        if(yDist<0) yDist = yDist*-1;
        return (xDist+yDist)*1;
        // distance heuristic gets multiplied by 500 to ensure the algorithm prioritizes points closer to the end
    }
    void retrace(std::pair<int, int> parent[26][26], int x, int y){
        if(x==startX && y==startY) return;
        QBrush cyanBrush(Qt::cyan);
        QPen outlinePen(Qt::black);
        if(x!=endX || y!=endY) this->addRect(20*(x), 20*(y), 20, 20, outlinePen, cyanBrush);
        retrace(parent, parent[x][y].first, parent[x][y].second);
    }
    void astar(){
        QBrush blueBrush(Qt::blue);
        QPen outlinePen(Qt::black);
        bool finished = false;
        // creates priority queue for pair<int, pair<int, int> > that prioritizes lower values
        std::priority_queue<std::pair <std::pair<int, int> , std::pair<int, int> >, std::vector<std::pair <std::pair<int, int> , std::pair<int, int> > >, std::greater<std::pair <std::pair<int, int> , std::pair<int, int> > > > minHeap;
        std::pair <std::pair<int, int>, std::pair<int, int> > start;
        int startH = distance(startX, startY);
        std::pair<int, int> p1 = std::make_pair(0, startH);
        std::pair<int, int> p2 = std::make_pair(startX, startY);
        start = std::make_pair(p1, p2);
        minHeap.push(start);
        bool found[26][26];
        int f[26][26];
        int g[26][26];
        int h[26][26];
        std::pair <int, int> parent[26][26];
        for(int i = 0; i<26; i++){
            for(int j = 0; j<26; j++){
                found[i][j]=false;
                f[i][j]=100000;
                g[i][j]=100000;
                h[i][j]=100000;
                parent[i][j] = std::make_pair (-1, -1);
            }
        }
        g[startX][startY]=0;
        h[startX][startY]=distance(startX, startY);
        f[startX][startY]=h[startX][startY];
        while(!minHeap.empty()){
            std::pair <std::pair<int, int>, std::pair<int, int> > p = minHeap.top();
            minHeap.pop();
            int x = p.second.first;
            int y = p.second.second;
            delay(25);
            found[x][y] = true;
            if(x==endX && y==endY){
                finished=true;
                break;
            }
            if(x!=startX || y!=startY){
                this->addRect(20*(x), 20*(y), 20, 20, outlinePen, blueBrush);
            }
            for(int dir = 0; dir<4; dir++){ // checks all 4 directions
                int i, j;
                if(dir==0){
                    i = x+1;
                    j = y;
                } else if (dir==1){
                    i = x;
                    j = y+1;
                } else if (dir==2){
                    i = x-1;
                    j = y;
                } else {
                    i = x;
                    j = y-1;
                }
                if(!valid(i, j)) continue; // checks if point is within the bounds of the grid
                if(found[i][j]==true) continue; // checks if the point has already been visited
                if(mainGrid[i][j]==3) continue; // checks if the point is a wall
                int newg = g[x][y]+1;
                int newh = distance(i, j);
                int newf = newg+newh;
                if(newf<f[i][j]){
                    minHeap.push(std::make_pair(std::make_pair(newf, newh), std::make_pair(i, j)));
                    g[i][j]=newg;
                    h[i][j]=newh;
                    f[i][j]=newf;
                    parent[i][j]=std::make_pair (x, y);
                }
            }
        }
        if(finished){
            retrace(parent, endX, endY);
        }
        qDebug() << "running a*";
        qDebug() << finished;
        return;
    }
    std::pair<int, int> minDist(int distance[26][26], bool found[26][26]){
        int minDist = 1000;
        int x = -1;
        int y = -1;
        for(int i = 0; i<26; i++){
            for(int j = 0; j<26; j++){
                if (found[i][j] == false && distance[i][j] <= minDist){
                    minDist = distance[i][j];
                    x = i, y = j;
                }
            }
        }
        return std::make_pair(x, y);
    }
    void dijkstra(){
        qDebug() << "Running Dijkstra's";
        QBrush blueBrush(Qt::blue);
        QPen outlinePen(Qt::black);
        bool finished = false;
        bool found[26][26];
        int distance[26][26];
        std::pair<int, int> parent[26][26];
        for(int i = 0; i<26; i++){
            for(int j = 0; j<26; j++){
                found[i][j]=false;
                distance[i][j]=10000;
                parent[i][j] = std::make_pair (-1, -1);
            }
        }
        distance[startX][startY]=0;
        while(found[endX][endY]==false){
            std::pair<int, int> u = minDist(distance, found);
            int x = u.first;
            int y = u.second;
            if(x==-1 && y==-1) break; // all possible spaces have been visited but still not found endpoint
            if(x==endX && y==endY){
                finished=true;
                break;
            }
            found[x][y]=true;
            delay(25);
            if(x!=startX || y!=startY){
                this->addRect(20*(x), 20*(y), 20, 20, outlinePen, blueBrush);
            }
            for(int dir = 0; dir<4; dir++){ // checks all 4 directions
                int i, j;
                if(dir==0){
                    i = x+1;
                    j = y;
                } else if (dir==1){
                    i = x;
                    j = y+1;
                } else if (dir==2){
                    i = x-1;
                    j = y;
                } else {
                    i = x;
                    j = y-1;
                }
                if(!valid(i, j)) continue; // space is outside of the boundaries of the grid
                if(mainGrid[i][j]==3) continue; // space is a wall
                if(!found[i][j] && distance[x][y]+1<distance[i][j]){
                    distance[i][j]=distance[x][y]+1;
                    parent[i][j]=std::make_pair(x, y);
                }
            }
        }
        if(finished){
            retrace(parent, endX, endY);
        }
        return;
    }
    void pathfinder(){
        int index = ui->algoTypeSelected();
        if(index==0) astar();
        else if (index==1) dijkstra();
        return;
    }
};
#endif // MAINWINDOW_H
