#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPainter>
#include"mypushbutton.h"
#include<QDebug>
#include<QTimer>
#include"gamescene.h"

MainScene::MainScene(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainScene)
{
    ui->setupUi(this);

    //配置主场景

    //设置固定大小
    setFixedSize(1200,887);

    //设置图标
    setWindowIcon(QIcon(":/res/iconflag.png"));

    //设置标题
    setWindowTitle("开始游戏界面");

    //退出按钮的实现
    connect(ui->actionQuit,&QAction::triggered,[=]{
        this->close();
    });

    //开始按钮
    MyPushButton* startBtn=new MyPushButton(":/res/playbutton.png");
    startBtn->setParent(this);
    //将按钮移动到屏幕中间
    startBtn->move(this->width()*0.5-startBtn->width()*0.5 , this->height()*0.4);

    //实例化游戏界面
    gameScene=new GameScene;

    //监听游戏界面中返回按钮的信号（注：该代码不需要写在下面的connect函数内部，因为该信号的创建只需要建立一次）
    connect(gameScene,&GameScene::gameSceneBack,this,[=](){
        //隐藏掉游戏场景
        gameScene->hide();
        //重新显示主场景
        this->show();
    });


    connect(startBtn,&MyPushButton::clicked,[=](){
        qDebug()<<"点击了开始按钮";
        //做弹起特效
        startBtn->zoom1();
        startBtn->zoom2();

        //进入游戏界面
        //延时进入——以便于能看到特效
        QTimer::singleShot(500,this,[=](){
            //自身隐藏
            this->hide();
            //显示游戏场景
            gameScene->show();            

        });

    });

}



//背景图片的加载
void MainScene::paintEvent(QPaintEvent*){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/mainscene.jpg");
    //对图像进行拉伸以填满整个屏幕
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

}

MainScene::~MainScene()
{
    delete ui;
}
