#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QMainWindow>
#include<QLabel>
#include"myqlabel.h"
//添加头文件——已改
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QEventLoop>
#include <QRect>
#include <QPoint>
#include <QApplication>
#include <QCursor>


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
    MyQLabel* Ice;
    QPixmap* bpixmap;
    QPixmap* bpixmapr;
    QPixmap* bpixmapl;

    //刷新频率
    int id1;
    //Fireman
    MyQLabel* Fire;
    //fireman的各种图片
    QPixmap* pixmap;
    QPixmap* pixmapr;
    QPixmap* pixmapl;

    //黄色机关
    MyQLabel * yOrgan;

    //紫色机关
    MyQLabel * pOrgan;

    //黄色开关
    MyQLabel * ySwitch;

    //紫色开关
    MyQLabel * pSwitch1;
    MyQLabel * pSwitch2;

    //成功函数
    void Victory();
    //失败函数
    void Failure();

    //判断输赢函数
    bool judge_win(double X,double Y,double bX,double bY);
    bool judge_lose(double X,double Y);
    bool judge_lose_ice(double X,double Y);

    QLabel* red_diamond;
    //红宝石的图片
    QPixmap* red_pixmap;

    QLabel* ice_diamond;
    //篮宝石的图片
    QPixmap* ice_pixmap;

    QLabel* box;
    //箱子的图片
    QPixmap* box_pixmap;


    //成功动画
    void GoUp(QLabel*,int,int);
    void WinFlash();
    //失败动画——已改
    //拷贝原因——方便动画中修改透明度
    QLabel* Cloud;
    QLabel* FireCopy;
    QLabel* IceCopy;
    //成功动画——已改
    QLabel *FireDoor,*IceDoor,*FireStair,*IceStair,*Top;
    QLabel *FireBack,*IceBack;
    //图片效果载入
    QPixmap cloud;
    QPixmap firedoor,icedoor,firestair,icestair,top;
    QPixmap fireback,iceback;
    void init();//初始化函数
    //失败图片
    QLabel* Fail;



    bool judge_floor(double X,double Y);
    int adjust_floor(double X,double Y);

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void leftMouseButtonClicked();

public slots:
    void handleMouseClick();

private:
    int isLeftClicked;


signals:
    //写一个自定义的信号，告诉主场景，点击了返回
    void gameSceneBack();

public slots:
};

#endif // GAMESCENE_H
