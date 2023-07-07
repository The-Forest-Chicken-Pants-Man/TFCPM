#include "myqlabel.h"
#include<QDebug>
#include<QPropertyAnimation>
#include<QPixmap>
#include<QString>

MyQLabel::MyQLabel(QString normalImg,QString pressImg)
{
    this->normalImgPath=normalImg;
    this->pressImgPath=pressImg;

    QPixmap pix;
    bool ret=pix.load(normalImg);
    if(!ret){
        qDebug()<<"图片加载失败";
        return ;
    }

    //设置图片的固定大小
    this->setFixedSize(pix.width(),pix.height());

    //设置不规则图片样式
//    this->setStyleSheet("QPushButton{border:0pix;}");

    //设置图片
    this->setPixmap(pix);

    //一大堆音效
    Platform = new QMediaPlayer(this);
    Platformout = new QAudioOutput(this);
    Platform ->setAudioOutput(Platformout);
    Platform ->setSource(QUrl("qrc:/res/Platform.mp3"));

    Lever = new QMediaPlayer(this);
    Leverout = new QAudioOutput(this);
    Lever ->setAudioOutput(Leverout);
    Lever ->setSource(QUrl("qrc:/res/Lever.mp3"));

    Pusher = new QMediaPlayer(this);
    Pusherout = new QAudioOutput(this);
    Pusher ->setAudioOutput(Pusherout);
    Pusher ->setSource(QUrl("qrc:/res/Pusher.mp3"));


    //初识化定时器对象
    timer1 = new QTimer(this);
    timer2 = new QTimer(this);

    //监听两者转换的信号并实现开关状态的转变
    connect(timer1,&QTimer::timeout,[=](){
        QPixmap pix;
        QString  str = QString(":/res/yellowbutton%1.png").arg(this->min++);
        pix.load(str);
        //设置图片的固定大小
        this->setFixedSize(pix.width(),pix.height());
        //设置图片
        this->setPixmap(pix);
        //判断转变结束将Min重置为1
        if(this->min>this->max){
            this->min=1;
            timer1->stop();
        }
    });
    connect(timer2,&QTimer::timeout,[=](){
        QPixmap pix;
        QString  str = QString(":/res/yellowbutton%1.png").arg(this->max--);
        pix.load(str);
        //设置图片的固定大小
        this->setFixedSize(pix.width(),pix.height());
        //设置图片
        this->setPixmap(pix);
        //判断转变结束将Min重置为1
        if(this->min>this->max){
            this->max=3;
            timer2->stop();
        }
    });

    //初识化定时器对象
    timer3 = new QTimer(this);
    timer4 = new QTimer(this);

    //监听两者转换的信号并实现开关状态的转变
    connect(timer3,&QTimer::timeout,[=](){
        QPixmap pix;
        QString  str = QString(":/res/purplebutton%1.png").arg(this->minn++);
        pix.load(str);
        //设置图片的固定大小
        this->setFixedSize(pix.width(),pix.height());
        //设置图片
        this->setPixmap(pix);
        //判断转变结束将Minn重置为1
        if(this->minn>this->maxx){
            this->minn=1;
            timer3->stop();
        }
    });
    connect(timer4,&QTimer::timeout,[=](){
        QPixmap pix;
        QString  str = QString(":/res/purplebutton%1.png").arg(this->maxx--);
        pix.load(str);
        //设置图片的固定大小
        this->setFixedSize(pix.width(),pix.height());
        //设置图片
        this->setPixmap(pix);
        //判断转变结束将maxx重置为3
        if(this->minn>this->maxx){
            this->maxx=3;
            timer4->stop();
        }
    });

    timer5 = new QTimer(this);
    timer6 = new QTimer(this);

    connect(timer5,&QTimer::timeout,[=](){
        if(this->hei>this->ini_hei )
        {hei-=1;Platform->play();}
        else
        {timer5->stop();Platform->stop();}
    });
    connect(timer6,&QTimer::timeout,[=](){
        if(this->hei<this->ini_hei+80 )
        {hei+=1;Platform->play();}
        else
        {timer6->stop();Platform->stop();}
    });

}

void MyQLabel::up(){
//    //创建动态对象
//    QPropertyAnimation * animation = new QPropertyAnimation(this,"geometry");

//    //动画时间间隔（ms为单位）
//    animation->setDuration(800);

//    //起始位置
//    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
//    //结束位置
//    animation->setEndValue(QRect(this->x(),this->y()-80,this->width(),this->height()));

//    //设置弹跳曲线
//    animation->setEasingCurve(QEasingCurve::Linear);

//    //执行动画
//    animation->start();

    timer5->start(10);
    timer6->stop();
}

void MyQLabel::down(){
//    //创建动态对象
//    QPropertyAnimation * animation = new QPropertyAnimation(this,"geometry");

//    //动画时间间隔（ms为单位）
//    animation->setDuration(800);

//    //起始位置
//    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
//    //结束位置
//    animation->setEndValue(QRect(this->x(),this->y()+80,this->width(),this->height()));

//    //设置弹跳曲线
//    animation->setEasingCurve(QEasingCurve::Linear);

//    //执行动画
//    animation->start();

    timer6->start(10);
    timer5->stop();

}

void MyQLabel::rightandleft(bool fl){
    this->flag=fl;
    Lever->play();
    if(this->flag)
        timer1->start(100);
    else
        timer2->start(100);
}

void MyQLabel::upanddown(bool fl){
    this->flagg=fl;
    Pusher->play();
    if(this->flagg)
    {
        timer4->stop();
        this->maxx=3;
        timer3->start(100);
        qDebug()<<"落";
    }
    else
    {
        timer3->stop();
        this->minn=1;
        timer4->start(100);
        qDebug()<<"起";
    }
}

void MyQLabel::init(){
    QPixmap pix;
    pix.load(this->normalImgPath);
    //设置图片的固定大小
    this->setFixedSize(pix.width(),pix.height());

    //设置图片
    this->setPixmap(pix);
}
