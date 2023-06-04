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
    //Fireman的坐标（初始坐标与当前坐标）
    double ini_x=109;
    double ini_y=650;
    double x=ini_x;
    double y=ini_y;

    //冰娃的坐标
    double b_ini_x=109;
    double b_ini_y=548;
    double bx=b_ini_x;
    double by=b_ini_y;
    QLabel* Ice;
    QPixmap* bpixmap;
    QPixmap* bpixmapr;
    QPixmap* bpixmapl;

    //刷新频率
    int id1;
    //Fireman
    QLabel* Fire;
    //fireman的各种图片
    QPixmap* pixmap;
    QPixmap* pixmapr;
    QPixmap* pixmapl;

    //成功函数
    void Victory();
    //失败函数
    void Failure();

    //判断输赢函数
    bool judge_win(double X,double Y,double bX,double bY);
    bool judge_lose(double X,double Y);
    bool judge_lose_ice(double X,double Y);

signals:
    //写一个自定义的信号，告诉主场景，点击了返回
    void gameSceneBack();

public slots:
};

#endif // GAMESCENE_H
