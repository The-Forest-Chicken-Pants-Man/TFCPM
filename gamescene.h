#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QMainWindow>
#include<QGraphicsPixmapItem>

class GameScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameScene(QWidget *parent = nullptr);

    //重写PaintEvent事件，画背景图
    void paintEvent(QPaintEvent*);

    //基本元素的声明
    QGraphicsPixmapItem Fireman;

signals:
    //写一个自定义的信号，告诉主场景，点击了返回
    void gameSceneBack();

public slots:
};

#endif // GAMESCENE_H
