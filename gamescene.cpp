#include "gamescene.h"
#include<QPainter>
#include"mypushbutton.h"
#include<QDebug>
#include<QTimer>
#include<QLabel>
#include<QKeyEvent>
#include<QtWidgets>

//x——横坐标，y——纵坐标

double vx;//速度
double vy;
double ax=4;//加速度
double ay=4;
double dx;//位移
double dy;

const double vx_max=5;
const double vy_max=20;

//fireman正脸的图像大小
const int z_Width=25;
const int z_Height=49;
//fireman侧脸的图像大小
const int c_Width=43;
const int c_Height=42;


GameScene::GameScene(QWidget *parent) : QMainWindow(parent)
{
    //配置场景
    this->setFixedSize(1000,739);

    //设置图标
    this->setWindowIcon(QPixmap(":/res/iconflag.png"));

    //设置标题
    this->setWindowTitle("森林冰火人");


    //状态按钮
    MyPushButton* stateBtn=new MyPushButton(":/res/statebutton.png");

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


    //Fireman的实现
    id1=startTimer(20);

    Fire = new QLabel(this);
    QPixmap* pixmap=new QPixmap(":/res/fireman2.png");


    //设置fireman的图片以及大小
    QImage image = pixmap->toImage();
    image = image.scaled(c_Width,c_Height);
    *pixmap = QPixmap::fromImage(image);
    qDebug()<<pixmap->width()<<pixmap->height();
    Fire->setPixmap(*pixmap);
    Fire->setFixedSize(c_Width,c_Height);

}

//同一块障碍物地板要高于墙一点，否则会卡bug  地板要比墙靠里一点点
bool judge_floor(double X,double Y)//判定地板
{
    if(Y>=650||(Y>=560&&Y<=580&&X>=0&&X<=330)||(Y>=570&&X>850))
        return 1;
    else
        return 0;
}

bool judge_leftwall(double X,double Y)//判定左墙
{
    if(X<=50||(X<=330&&X>=320&&Y>=575&&Y<=585))
        return 1;
    else
        return 0;
}
bool judge_rightwall(double X,double Y)//判定右墙
{
    if((X>=845&&X<=855&&Y>=580)||X>=950)
        return 1;
    else
        return 0;
}
bool judge_head(int X,int Y)//判定头是否撞墙
{
    if(Y>=585&&Y<=610&&X>=0&&X<=330)
        return 1;
    else
        return 0;
}

void GameScene::timerEvent(QTimerEvent *time){
    if(time->timerId()==id1)
    {
        int modex= 0;
        int modey= 0;
        int t=1;
        if(GetAsyncKeyState('A')&0x8000)modex--;
        if(GetAsyncKeyState('W')&0x8000)modey++;
        if(GetAsyncKeyState('D')&0x8000)modex++;
        if(modex==0)
        {
            vx=0;
        }
        if(modex==-1){
            if(judge_leftwall(x,y)==1){
                vx=0;
            }
            else{
                if(vx>=-vx_max){
                    vx-=ax/2;
                }
                else{
                    vx=-vx_max;
                }
            }
        }
        if(modex==1){
            if(judge_rightwall(x,y)==1){
                vx=0;
            }
            else{
                if(vx<=vx_max){
                    vx+=ax/2;
                }
                else{
                    vx=vx_max;
                }
            }
        }

        if(judge_floor(x,y))
        {
            if(modey==1)
                vy=vy_max;
            else
            {
                vy=0;
                ay=0;
            }
            t=1;
        }
        if(judge_floor(x,y)==0)
        {
            if(judge_head(x,y)==1){
                vy=0;
            }
            if(vy<=20&&vy>=-20)
            {
                vy-=ay;
                ay=4;
            }
            if(vy>20)vy=20;
            if(vy<-20)vy=-20;
        }

        dx+=vx;
        dy+=vy*t+ay*t*t/2;
        t++;
        x+=dx;
        y-=dy;
        dx=0,dy=0;
        Fire->setGeometry(x,y,Fire->width(),Fire->height());
        //QImage *img=new QImage;
        //img->load(":/res/fire.png");
        //可以在此处改变形态
        modex=0;
        modey=0;
        //Fire->setPixmap(QPixmap::fromImage(*img));
        Fire->show();

    }
};

//背景图片的加载
void GameScene::paintEvent(QPaintEvent*){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/gamescene.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

}
