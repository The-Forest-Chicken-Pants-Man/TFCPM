#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    //构造函数——传入按压前和按压后的图片
    MyPushButton(QString normalImg,QString pressImg="");

    QString normalImgPath;
    QString pressImgPath;

    //弹跳效果
    void zoom1();//向下跳
    void zoom2();//向上跳

signals:

public slots:
};

#endif // MYPUSHBUTTON_H
