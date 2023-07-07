#ifndef MYQLABEL_H
#define MYQLABEL_H

#include <QLabel>
#include <QObject>
#include <QString>
#include<QTimer>
#include<QMediaPlayer>
#include<QAudioOutput>

class MyQLabel : public QLabel
{
public:
    //构造函数——传入按压前和按压后的图片
    MyQLabel(QString normalImg,QString pressImg="");

    QString normalImgPath;
    QString pressImgPath;

    //初识化函数
    void init();

    //上下移动效果——两种机关上下移动的范围均为80pixel，故写成一个函数
    void up();
    void down();
    double hei;
    double ini_hei;
    QTimer * timer5;
    QTimer * timer6;

    //黄色开关左右搬动的效果
    bool flag=true;
    void rightandleft(bool fl);
    QTimer * timer1;
    QTimer * timer2;
    int min=1;
    int max=3;

    //紫色开关上下按压的效果
    bool flagg=true;
    void upanddown(bool fl);
    QTimer * timer3;
    QTimer * timer4;
    int minn=1;
    int maxx=3;

    //一大堆音效
    QMediaPlayer * Platform;
    QAudioOutput * Platformout;
    QMediaPlayer * Pusher;
    QAudioOutput * Pusherout;
    QMediaPlayer * Lever;
    QAudioOutput * Leverout;

};

#endif // MYQLABEL_H
