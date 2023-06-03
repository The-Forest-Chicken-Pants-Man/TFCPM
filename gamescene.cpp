#include "gamescene.h"
#include<QPainter>
#include"mypushbutton.h"
#include<QDebug>
#include<QTimer>

GameScene::GameScene(QWidget *parent) : QMainWindow(parent)
{
    //配置场景
    this->setFixedSize(1000,600);

    //设置图标
    this->setWindowIcon(QPixmap(":/res/box.jpg"));

    //设置标题
    this->setWindowTitle("森林冰火人");


    //状态按钮
    MyPushButton* stateBtn=new MyPushButton(":/res/button.png");

    stateBtn->setParent(this);
    stateBtn->move(this->width()*0.5-stateBtn->width()*0.5,this->height()-stateBtn->height());

    connect(stateBtn,&QPushButton::clicked,[=](){
        qDebug()<<"点击了状态按钮";
        stateBtn->zoom1();
        stateBtn->zoom2();
        //延时返回到主场景
        QTimer::singleShot(500,this,[=](){
            //告诉主场景，我返回了，主场景监听gameScene的返回按钮
            emit this->gameSceneBack();
        });
    });



}



//背景图片的加载
void GameScene::paintEvent(QPaintEvent*){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/scene0.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

}
