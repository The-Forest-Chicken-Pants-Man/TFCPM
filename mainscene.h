#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QMainWindow>
#include"gamescene.h"
#include<QMediaPlayer>
#include<QAudioOutput>

namespace Ui {
class MainScene;
}

class MainScene : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainScene(QWidget *parent = nullptr);
    ~MainScene();

    //重写PaintEvent事件，画背景图
    void paintEvent(QPaintEvent*);

    GameScene* gameScene=NULL;

   //设置背景音乐
    QMediaPlayer * mainBackgroundPlayer;
    QAudioOutput * mainBackgroundAudioOutput;
    //背景音乐的实现
    QMediaPlayer * gameBackgroundPlayer;
    QAudioOutput * gameBackgroundAudioOutput;

private:
    Ui::MainScene *ui;
};

#endif // MAINSCENE_H
