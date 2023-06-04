#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QMainWindow>
#include<QLabel>

class GameScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameScene(QWidget *parent = nullptr);

    //重写PaintEvent事件，画背景图
    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent *time);
    //Fireman的坐标
    double x=109;
    double y=600;
    //刷新频率
    int id1;
    //Fireman
    QLabel* Fire;
    QLabel* player;

signals:
    //写一个自定义的信号，告诉主场景，点击了返回
    void gameSceneBack();

public slots:
};

#endif // GAMESCENE_H
