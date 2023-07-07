#include "gamescene.h"
#include<QPainter>
#include"mypushbutton.h"
#include<QDebug>
#include<QTimer>
#include<QLabel>
#include<QKeyEvent>
#include<QtWidgets>
#include<QMessageBox>
#include<QLabel>

//x——横坐标，y——纵坐标

double vx;//速度
double vy;
double ax=4;//加速度
double ay=0;
double dx;//位移
double dy;

double bvx;//速度
double bvy;
double bax=4;//加速度
double bay=0;
double bdx;//位移
double bdy;

double lww=10;
double lwh=30;

const double vx_max=6;
const double vy_max=12;
const double ay_max=1;

//fireman的图像大小
const double Width=74;
const double Height=58;

double height=Height;
double width=18;
double width1=37;
double height1=0;

//黄色可移动楼梯的上下移动幅度
bool yellowbuttonflag=true;//黄色楼梯位于上面
int purplebuttonflag1= 0;//紫色按钮拥有四种状态  0——无人踩  1——被火娃踩下  2——被冰娃踩下  3——被两人同时踩下
int purplebuttonflag2=0;
bool purplebuttonflag=true;

const double box_ini_x=623;
const double box_ini_y=228;
double boxx=box_ini_x;
double boxy=box_ini_y;
double boxdx;
double boxvx;//箱子速度
double boxax;

bool red_diamond_flag1=1;
bool ice_diamond_flag1=1;
bool red_diamond_flag2=1;
bool ice_diamond_flag2=1;
bool red_diamond_flag3=1;
bool ice_diamond_flag3=1;
bool red_diamond_flag4=1;
bool ice_diamond_flag4=1;


bool invincible=false;//是否让两人无敌——方便debug


//构造函数部分
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
        init();
        //延时返回到主场景
        QTimer::singleShot(10,this,[=](){
            //告诉主场景，我返回了，主场景监听gameScene的返回按钮
            emit this->gameSceneBack();

        });
    });


    //黄色机关和紫色机关的导入
    yOrgan = new MyQLabel (":/res/yelloworgan.png");
    yOrgan->setParent(this);
    yOrgan->hei=385;
    yOrgan->ini_hei=385;
    //    yOrgan->move(38,385);
    //    yOrgan->show();
    pOrgan = new MyQLabel (":/res/purpleorgan.png");
    pOrgan->setParent(this);
    pOrgan->hei=313;
    pOrgan->ini_hei=313;
    //    pOrgan->move(864,313);
    //    pOrgan->show();


    //黄色开关和紫色开关的导入
    ySwitch = new MyQLabel(":/res/yellowbutton1.png");
    ySwitch->setParent(this);
    ySwitch->move(250,475);
    ySwitch->show();
    pSwitch1 = new MyQLabel(":/res/purplebutton1.png");
    pSwitch1->setParent(this);
    pSwitch1->move(270,373);
    pSwitch1->show();
    pSwitch2 = new MyQLabel(":/res/purplebutton1.png");
    pSwitch2->setParent(this);
    pSwitch2->move(740,274);
    pSwitch2->show();


    //刷新率
    id1=startTimer(20);

    Fire = new MyQLabel(":/res/fireman1.png");
    Fire->setParent(this);
    Ice=new MyQLabel(":/res/icegirl1.png");
    Ice->setParent(this);

    //fireman各种图片的导入
    pixmap=new QPixmap(":/res/fireman1.png");
    QImage image1 = pixmap->toImage();
    image1 = image1.scaled(pixmap->size() * 0.7, Qt::KeepAspectRatio);
    *pixmap = QPixmap::fromImage(image1);

    pixmapr=new QPixmap(":/res/fireman2.png");
    QImage image2 = pixmapr->toImage();
    image2 = image2.scaled(pixmapr->size() * 0.7, Qt::KeepAspectRatio);
    *pixmapr = QPixmap::fromImage(image2);

    pixmapl=new QPixmap(":/res/fireman3.png");
    QImage image3 = pixmapl->toImage();
    image3 = image3.scaled(pixmapl->size() * 0.7, Qt::KeepAspectRatio);
    *pixmapl = QPixmap::fromImage(image3);

    //icegirl各种图片的导入
    bpixmap=new QPixmap(":/res/icegirl1.png");
    QImage image4 = bpixmap->toImage();
    image4 = image4.scaled(bpixmap->size() * 0.7, Qt::KeepAspectRatio);
    *bpixmap = QPixmap::fromImage(image4);

    bpixmapr=new QPixmap(":/res/icegirl2.png");
    QImage image5 = bpixmapr->toImage();
    image5 = image5.scaled(bpixmapr->size() * 0.7, Qt::KeepAspectRatio);
    *bpixmapr = QPixmap::fromImage(image5);

    bpixmapl=new QPixmap(":/res/icegirl3.png");
    QImage image6 = bpixmapl->toImage();
    image6 = image6.scaled(bpixmapl->size() * 0.7, Qt::KeepAspectRatio);
    *bpixmapl = QPixmap::fromImage(image6);

    red_diamond1=new QLabel(this);
    //red_diamond图像导入
    red_pixmap1=new QPixmap(":/res/red_diamond.png");
    QImage image_red1 = red_pixmap1->toImage();
    image_red1 = image_red1.scaled(red_pixmap1->size() * 0.8, Qt::KeepAspectRatio);
    *red_pixmap1 = QPixmap::fromImage(image_red1);

    red_diamond2=new QLabel(this);
    //red_diamond图像导入
    red_pixmap2=new QPixmap(":/res/red_diamond.png");
    QImage image_red2 = red_pixmap2->toImage();
    image_red2 = image_red2.scaled(red_pixmap2->size() * 0.8, Qt::KeepAspectRatio);
    *red_pixmap2 = QPixmap::fromImage(image_red2);

    red_diamond3=new QLabel(this);
    //red_diamond图像导入
    red_pixmap3=new QPixmap(":/res/red_diamond.png");
    QImage image_red3 = red_pixmap3->toImage();
    image_red3 = image_red3.scaled(red_pixmap3->size() * 0.8, Qt::KeepAspectRatio);
    *red_pixmap3 = QPixmap::fromImage(image_red3);

    red_diamond4=new QLabel(this);
    //red_diamond图像导入
    red_pixmap4=new QPixmap(":/res/red_diamond.png");
    QImage image_red4 = red_pixmap4->toImage();
    image_red4 = image_red4.scaled(red_pixmap4->size() * 0.8, Qt::KeepAspectRatio);
    *red_pixmap4 = QPixmap::fromImage(image_red4);

    ice_diamond1=new QLabel(this);
    //ice_diamond图像导入
    ice_pixmap1=new QPixmap(":/res/ice_diamond.png");
    QImage image_ice1 = ice_pixmap1->toImage();
    image_ice1 = image_ice1.scaled(ice_pixmap1->size() * 0.8, Qt::KeepAspectRatio);
    *ice_pixmap1 = QPixmap::fromImage(image_ice1);

    ice_diamond2=new QLabel(this);
    //ice_diamond图像导入
    ice_pixmap2=new QPixmap(":/res/ice_diamond.png");
    QImage image_ice2 = ice_pixmap2->toImage();
    image_ice2 = image_ice2.scaled(ice_pixmap2->size() * 0.8, Qt::KeepAspectRatio);
    *ice_pixmap2 = QPixmap::fromImage(image_ice2);

    ice_diamond3=new QLabel(this);
    //ice_diamond图像导入
    ice_pixmap3=new QPixmap(":/res/ice_diamond.png");
    QImage image_ice3 = ice_pixmap3->toImage();
    image_ice3 = image_ice3.scaled(ice_pixmap3->size() * 0.8, Qt::KeepAspectRatio);
    *ice_pixmap3 = QPixmap::fromImage(image_ice3);

    ice_diamond4=new QLabel(this);
    //ice_diamond图像导入
    ice_pixmap4=new QPixmap(":/res/ice_diamond.png");
    QImage image_ice4 = ice_pixmap4->toImage();
    image_ice4 = image_ice4.scaled(ice_pixmap4->size() * 0.8, Qt::KeepAspectRatio);
    *ice_pixmap4 = QPixmap::fromImage(image_ice4);

    box=new QLabel(this);
    //ice_diamond图像导入
    box_pixmap=new QPixmap(":/res/box.jpg");
    QImage image_box = box_pixmap->toImage();
    //image_box = image_ice.scaled(ice_pixmap->size() * 0.8, Qt::KeepAspectRatio);
    *box_pixmap = QPixmap::fromImage(image_box);


    //固定fireman的大小
    Fire->setFixedSize(Width,Height);
    Ice->setFixedSize(Width,Height);
    box->setFixedSize(60,57);


    //云雾图片导入
    cloud.load(":/res/cloud.png");
    QImage image_cloud=cloud.toImage();
    image_cloud=image_cloud.scaled(cloud.size()*0.1, Qt::KeepAspectRatio);
    cloud=QPixmap::fromImage(image_cloud);

    //门与楼梯图片导入
    //备注：需要顶端的图片，原因是门需要向上升起并被覆盖消失，但动画并不具有这个功能
    firedoor.load(":/res/FireDoor.png");
    QImage image_firedoor=firedoor.toImage();
    image_firedoor=image_firedoor.scaled(firedoor.size()*0.2, Qt::KeepAspectRatio);
    firedoor=QPixmap::fromImage(image_firedoor);

    icedoor.load(":/res/IceDoor.png");
    QImage image_icedoor=icedoor.toImage();
    image_icedoor=image_icedoor.scaled(icedoor.size()*0.2, Qt::KeepAspectRatio);
    icedoor=QPixmap::fromImage(image_icedoor);

    firestair.load(":/res/stairs.png");
    QImage image_firestair=firestair.toImage();
    image_firestair=image_firestair.scaled(firestair.size()*0.215, Qt::KeepAspectRatio);
    firestair=QPixmap::fromImage(image_firestair);

    icestair.load(":/res/stairs.png");
    QImage image_icestair=icestair.toImage();
    image_icestair=image_icestair.scaled(icestair.size()*0.215, Qt::KeepAspectRatio);
    icestair=QPixmap::fromImage(image_icestair);

    top.load(":/res/top.png");
    QImage image_top=top.toImage();
    image_top=image_top.scaled(top.size()*0.198, Qt::KeepAspectRatio);
    top=QPixmap::fromImage(image_top);

    //门与楼梯对象初始化

    FireStair=new QLabel(this);
    FireStair->setFixedSize(43,55);
    FireStair->setPixmap(firestair);
    FireStair->move(801,100);

    IceStair=new QLabel(this);
    IceStair->setFixedSize(43,55);
    IceStair->setPixmap(icestair);
    IceStair->move(876,100);

    FireDoor=new QLabel(this);
    FireDoor->setFixedSize(43,55);
    FireDoor->setPixmap(firedoor);
    FireDoor->move(801,100);

    IceDoor=new QLabel(this);
    IceDoor->setFixedSize(43,55);
    IceDoor->setPixmap(icedoor);
    IceDoor->move(876,100);

    Top=new QLabel(this);
    Top->setFixedSize(122,102);
    Top->setPixmap(top);
    Top->move(802,0);
    Fire->raise();
    Ice->raise();
    Top->raise();//保证在图层最上层具有遮挡功能

    //背影导入
    fireback.load(":/res/fireman5.png");
    QImage image_fireback=fireback.toImage();
    image_fireback=image_fireback.scaled(fireback.size()*0.6, Qt::KeepAspectRatio);
    fireback=QPixmap::fromImage(image_fireback);

    iceback.load(":/res/icegirl5.png");
    QImage image_iceback=iceback.toImage();
    image_iceback=image_iceback.scaled(iceback.size()*0.6, Qt::KeepAspectRatio);
    iceback=QPixmap::fromImage(image_iceback);

    FireBack=new QLabel(this);
    FireBack->setFixedSize(32,50);
    FireBack->setPixmap(fireback);
    FireBack->move(806,103);
    FireBack->hide();

    IceBack=new QLabel(this);
    IceBack->setFixedSize(32,50);
    IceBack->setPixmap(iceback);
    IceBack->move(880,103);
    IceBack->hide();

    // 初始化 Cloud 对象
    Cloud=new QLabel(this);
    Cloud->setPixmap(cloud);
    Cloud->move(0,1000);
    Cloud->show();


    // 初始化 FireCopy 对象
    FireCopy=new QLabel(this);
    FireCopy->setFixedSize(Width,Height);
    FireCopy->setPixmap(*pixmap);
    FireCopy->move(0,1000);
    FireCopy->show();

    //初始化IceCopy对象
    IceCopy=new QLabel(this);
    IceCopy->setFixedSize(Width,Height);
    IceCopy->setPixmap(*bpixmap);
    IceCopy->move(0,1000);
    IceCopy->show();

    Top->raise();//保证在图层最上层具有遮挡功能

    //失败图片载入
    QPixmap fail;
    fail.load(":/res/Fail.png");
    Fail=new QLabel(this);
    Fail->setPixmap(fail);
    Fail->setFixedSize(900,673);
    Fail->move(50,0);
    Fail->hide();
    //成功图片导入——改动
    QPixmap win;
    win.load(":/res/Win.png");
    QImage image_win=win.toImage();
    image_win=image_win.scaled(win.size()*1.06, Qt::KeepAspectRatio);
    win=QPixmap::fromImage(image_win);
    Win=new QLabel(this);
    Win->setPixmap(win);
    Win->setFixedSize(900,673);
    Win->move(50,0);
    Win->hide();


    connect(this, &GameScene::leftMouseButtonClicked, this, &GameScene::handleMouseClick);


    //各种音效的导入
    Death = new QMediaPlayer(this);
    Deathout = new QAudioOutput(this);
    Death ->setAudioOutput(Deathout);
    Death ->setSource(QUrl("qrc:/res/Death.mp3"));

    Diamond = new QMediaPlayer(this);
    Diamondout = new QAudioOutput(this);
    Diamond ->setAudioOutput(Diamondout);
    Diamond ->setSource(QUrl("qrc:/res/Diamond.mp3"));

    jump_fire = new QMediaPlayer(this);
    jump_fireout = new QAudioOutput(this);
    jump_fire ->setAudioOutput(jump_fireout);
    jump_fire ->setSource(QUrl("qrc:/res/Jump_wg.mp3"));

    jump_ice = new QMediaPlayer(this);
    jump_iceout = new QAudioOutput(this);
    jump_ice ->setAudioOutput(jump_iceout);
    jump_ice ->setSource(QUrl("qrc:/res/Jump_fb.mp3"));

    Door = new QMediaPlayer(this);
    Doorout = new QAudioOutput(this);
    Door ->setAudioOutput(Doorout);
    Door ->setSource(QUrl("qrc:/res/Door.mp3"));


}


//同一块障碍物地板要高于墙一点，否则会卡bug  地板要比墙靠里一点点
bool GameScene::judge_floor(double X,double Y)//判定地板
{
    if((Y>=708-Height&&X<=466-width1)/*底层*/
        ||(Y>=722-Height&&X>=466-width1&&X<=584-width1)/*底层火*/
        ||(Y>=708-Height&&X>=584-width1&&X<=665-width1)/*底层中*/
        ||(Y>=722-Height&&X>=664-width1&&X<=785-width1)/*底层水*/
        ||(Y>=708-Height&&X>=785-width1)/*底层*/
        ||(Y>=606-Height&&Y<=640-Height&&X>=0-width1&&X<=340-width1)/*一层夹层墙*/
        ||(Y>=628-Height&&X>882-width1)/*一层右边墙*/
        ||(Y>=509-Height&&Y<=530-Height&&X<=490-width1)/*二层高墙*/
        ||(Y>=yOrgan->hei-Height&&Y<=yOrgan->hei+22-Height&&X<=141-width1)/*黄色机关墙*/
        ||(Y>=557-Height&&Y<=580-Height&&X>=438-width1&&X<=830-width1)/*二层低墙*/
        ||((Y>=385-Height)&&(Y<=400-Height)&&X>=136-width1&&X<=562-width1)/*三层左墙*/
        ||((Y>=410-Height)&&(Y<=430-Height)&&X>=561-width1)/*三层右墙*/
        ||(Y>=pOrgan->hei-Height&&Y<=pOrgan->hei+22-Height&&X>=864-width1)/*紫色机关*/
        ||(Y>=286-Height&&Y<=311-Height&&X<=860-width1)||(Y>=186-Height&&Y<=297-Height&&X<=168-width1)/*四层三个墙*/
        ||(((Y>=boxy-Height)&&(Y<=boxy+20-Height)&&X>=boxx-width1&&X<=boxx+width1))/*箱子上面*/
        ||(Y>=160-Height&&Y<=185-Height&&X>=254-width1)/*第四层*/
        )
        return 1;
    else
        return 0;
}
int GameScene::adjust_floor(double X,double Y){
    if(Y>=708-Height&&X<=466-width1)        return 708-Height;
    else if(Y>=722-Height&&X>=466-width1&&X<=584-width1)        return 722-Height;
    else if(Y>=708-Height&&X>=584-width1&&X<=665-width1)        return 708-Height;
    else if(Y>=722-Height&&X>=664-width1&&X<=785-width1)        return 722-Height;
    else if(Y>=708-Height&&X>=785-width1)        return 708-Height;
    else if(Y>=606-Height&&Y<=640-Height&&X>=0-width1&&X<=340-width1)        return 606-Height;
    else if(Y>=628-Height&&X>882-width1)        return 628-Height;
    else if(Y>=509-Height&&Y<=530-Height&&X<=490-width1)        return 509-Height;
    else if(Y>=yOrgan->hei-Height&&Y<=yOrgan->hei+22-Height&&X<=141-width1)        return yOrgan->hei-Height;
    else if(Y>=557-Height&&Y<=580-Height&&X>=438-width1&&X<=830-width1)        return 557-Height;
    else if((Y>=385-Height)&&(Y<=400-Height)&&X>=136-width1&&X<=562-width1)        return 385-Height;
    else if((Y>=410-Height)&&(Y<=430-Height)&&X>=561-width1)        return 410-Height;
    else if(Y>=pOrgan->hei-Height&&Y<=pOrgan->hei+22-Height&&X>=864-width1)      return pOrgan->hei-Height;
    else if(Y>=286-Height&&Y<=311-Height&&X<=860-width1)        return 286-Height;
    else if(Y>=247-Height&&Y<=282-Height&&X<=212-width1)       return 247-Height;
    else if(Y>=186-Height&&Y<=297-Height&&X<=168-width1)        return 186-Height;
    else if(Y>=160-Height&&Y<=185-Height&&X>=254-width1)        return 160-Height;
    else if((Y>=boxy-Height)&&(Y<=boxy+20-Height)&&X>=boxx-width1&&X<=boxx+width1) return boxy-Height;
    else return Y;
}
bool GameScene::judge_leftwall(double X,double Y)//判定左墙
{
    if(X<=38-18/*左边全墙*/
        ||(X<=330&&X>=320&&Y>=608-58&&Y<=634-58)/*一层中间卡墙*/
        ||(Y>=560&&Y<=588&&X>=842-lww&&X<=842)/*二层右墙*/
        ||(Y>=512-50&&Y<=557-50&&X>=480-18-lww&&X<=480-18)/*二层中间墙*/
        ||(!yellowbuttonflag&&Y>=463-lwh&&Y<=510-lwh&&X<=120) /*二层机关右边*/
        ||(Y>=385-50&&Y<=430-50&&X>=560-18-lww&&X<=560-18)/*三层中间墙*/
        //        ||(Y>=243-50&&Y<=282-50&&X>=205-18-lww&&X<=205-18)/*箱子左边*/
        ||(Y>=288-50&&Y<=332-50&&X>=866-18-lww-30&&X<=866-18)/*紫色机关左墙*/
        ||(Y>=188-50&&Y<=241-50&&X>=166-18-lww&&X<=166-18)/*箱子上面的左墙*/
        ||(boxx<=168&&X<=168+60-18-lww+10&&Y<=282-Height+30&&Y>=282-57-Height)/*箱子是否撞到左边墙*/
        ||(Y>=yOrgan->hei-50&&Y<=yOrgan->hei+22-50&&X<=38+98-18)/*黄色机关右边*/
        )
        return 1;
    else
        return 0;
}
bool GameScene::judge_rightwall(double X,double Y)//判定右墙
{
    if((X>=835&&X<=855&&Y>=580)/*一层右墙*/
        ||X>=950-40/*右侧墙*/
        ||(Y<=pOrgan->hei+18&&Y>=pOrgan->hei-83&&X>=960-96-43&&X<=960-96-43+5)/*紫色机关左墙*/
        ||(Y<=415-10&&Y>=386-83+20&&X>=140-43&&X<=140-43+10)/*三层左短墙*/
        ||(Y<=189-10&&Y>=161-83+20&&X>=256-43&&X<=256-43+10)/*最上层左短墙*/
        )
        return 1;
    else
        return 0;
}
bool GameScene::judge_head(int X,int Y)//判定头是否撞墙
{
    if((Y>=585&&Y<=610&&X>=0&&X<=338-10)/*一层初始上墙*/
        ||(Y>=557-height1&&Y<=588-height1&&X>=409&&X<=830)/*一层上墙*/
        ||Y<=42/*上顶墙*/
        ||(Y>=160&&Y<=185&&X>=254-30)/*四层上墙*/
        ||(Y>=511-height1&&Y<=534-height1&&X<=423)/*一层夹层上墙*/
        ||(Y>=562-height1&&Y<=590-height1&&X>=441&&X<=830)/*一层上墙*/
        ||(Y>=379-height1&&Y<=414-height1&&X>=140-60&&X<=520)/*二层左墙*/
        ||(Y>=409-height1&&Y<=434-height1&&X>=520&&X<=962)/*二层右墙*/
        ||(yellowbuttonflag&&Y>=379-58&&Y<=420-58&&X<=141-width1)/*黄色机关墙——需要debug*/
        ||(Y>=260-height1&&Y<=318-height1&&X>=0&&X<=717)/*三层左墙*/
        ||(Y>=288-height1&&Y<=337-height1&&X>=713&&X<=861-50)/*三层右墙*/
        ||(Y>=pOrgan->hei-height1&&Y<=pOrgan->hei+18-height1&&X>=863-20)/*紫色机关上面*/
        ||(Y>=yOrgan->hei-height1&&Y<=yOrgan->hei+22-height1&&X<=141-10)/*黄色机关上面*/
        )
        return 1;
    else
        return 0;
}

bool judge_fire(double X,double Y)//岩浆判定
{
    if(Y>=708-height&&X>=466-width&&X<=560-width)
        return 1;
    else
        return 0;
}
bool judge_poison(double X,double Y)//毒液判定
{
    if(Y>=557-height&&Y<=580-height&&X>=625-width1&&X<=725-width)
        return 1;
    else
        return 0;
}
bool judge_water(double X,double Y)//水判定
{
    if(Y>=708-height&&X>=670-width1&&X<=780-width1)
        return 1;
    else
        return 0;
}

//成功动画
void GameScene::GoUp(QLabel *label, int startX, int startY)
{
    // 设置初始位置
    label->move(startX, startY);

    // 创建动画效果
    QPropertyAnimation *animation = new QPropertyAnimation(label, "geometry");
    animation->setDuration(1000); // 设置动画持续时间（以毫秒为单位）
    animation->setStartValue(QRect(startX, startY, label->width(), label->height()));
    animation->setEndValue(QRect(startX, startY - 60, label->width(), label->height())); // 向上淡出

    // 创建透明度动画效果
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(label);
    label->setGraphicsEffect(opacityEffect);

    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    opacityAnimation->setDuration(1000);
    opacityAnimation->setStartValue(1.0);
    opacityAnimation->setEndValue(0);

    // 创建动画组
    QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup;
    animationGroup->addAnimation(animation);
    animationGroup->addAnimation(opacityAnimation);

    // 启动动画
    animationGroup->start();
}
void GameScene::WinFlash(){
    //成功时门上升动画

    Door->play();
    // 实例化 label2 和 label3 的位置动画对象
    QPropertyAnimation *animation2 = new QPropertyAnimation(FireDoor, "pos");
    QPropertyAnimation *animation3 = new QPropertyAnimation(IceDoor, "pos");

    // 设置 label2 和 label3 的位置动画属性
    int animationDuration = 1000; // 动画持续时间（ms）
    int yOffset = 60; // 向上移动的距离

    animation2->setDuration(animationDuration);
    animation2->setStartValue(QPoint(801, 100));
    animation2->setEndValue(QPoint(801, 100-yOffset));
    animation2->setEasingCurve(QEasingCurve::InOutQuart);

    animation3->setDuration(animationDuration);
    animation3->setStartValue(QPoint(876, 100));
    animation3->setEndValue(QPoint(876, 100-yOffset));
    animation3->setEasingCurve(QEasingCurve::InOutQuart);

    QPropertyAnimation *animation4 = new QPropertyAnimation(Fire, "pos");
    QPropertyAnimation *animation5 = new QPropertyAnimation(Ice, "pos");

    animation4->setDuration(1000);
    animation4->setStartValue(QPoint(Fire->x(), Fire->y()));
    animation4->setEndValue(QPoint(Fire->x(), Fire->y()));
    //animation2->setEasingCurve(QEasingCurve::InOutQuart);

    animation5->setDuration(100);
    animation5->setStartValue(QPoint(Ice->x(), Ice->y()));
    animation5->setEndValue(QPoint(Ice->x(), Ice->y()));
    //animation3->setEasingCurve(QEasingCurve::InOutQuart);


    // 启动动画
    animation2->start();
    animation3->start();
    animation4->start();
    animation5->start();

    // 创建并启动 QEventLoop，等待动画完成
    QEventLoop loop;
    QObject::connect(animation3, &QParallelAnimationGroup::finished, &loop, &QEventLoop::quit);
    loop.exec();

    //成功时人物走上楼梯的动画
    Fire->hide();
    Ice->hide();
    FireBack->show();
    IceBack->show();
    GoUp(FireBack,FireBack->x(),FireBack->y());
    GoUp(IceBack,IceBack->x(),IceBack->y());

    QEventLoop loop2;
    //创建单次定时器，槽函数为事件循环的退出函数
    QTimer::singleShot(int(1000), &loop2, SLOT(quit()));
    //事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
    loop2.exec();
}

//云雾动画
void fadeOutAnimation(QLabel *label, int startX, int startY)
{
    // 设置初始位置
    label->move(startX, startY);

    // 创建动画效果
    QPropertyAnimation *animation = new QPropertyAnimation(label, "geometry");
    animation->setDuration(1000); // 设置动画持续时间（以毫秒为单位）
    animation->setStartValue(QRect(startX, startY, label->width(), label->height()));
    animation->setEndValue(QRect(startX, startY - 100, label->width(), label->height())); // 向上淡出

    // 创建透明度动画效果
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(label);
    label->setGraphicsEffect(opacityEffect);

    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    opacityAnimation->setDuration(1000);
    opacityAnimation->setStartValue(1.0);
    opacityAnimation->setEndValue(0.0);

    // 创建动画组
    QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup;
    animationGroup->addAnimation(animation);
    animationGroup->addAnimation(opacityAnimation);

    // 启动动画
    animationGroup->start();

    // 创建并启动 QEventLoop，等待动画完成
    QEventLoop loop;
    QObject::connect(animationGroup, &QParallelAnimationGroup::finished, &loop, &QEventLoop::quit);
    loop.exec();
}
//人物消失动画
void ManfadeOutAnimation(QLabel *label, int startX, int startY)
{
    // 设置初始位置
    label->move(startX, startY);

    // 创建动画效果
    QPropertyAnimation *animation = new QPropertyAnimation(label, "geometry");
    animation->setDuration(300); // 设置动画持续时间（以毫秒为单位）
    animation->setStartValue(QRect(startX, startY, label->width(), label->height()));
    animation->setEndValue(QRect(startX, startY + 15, label->width(), label->height())); // 向下淡出

    // 创建透明度动画效果
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(label);
    label->setGraphicsEffect(opacityEffect);

    QPropertyAnimation *opacityAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    opacityAnimation->setDuration(300);
    opacityAnimation->setStartValue(1.0);
    opacityAnimation->setEndValue(0.0);

    // 创建动画组
    QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup;
    animationGroup->addAnimation(animation);
    animationGroup->addAnimation(opacityAnimation);

    // 启动动画
    animationGroup->start();
}

bool GameScene::judge_win(double X,double Y,double bX,double bY){
    if(X>=790-10&&X<=856-70&&Y>=80&&Y<=156&&bY>=80&&bY<=156&&bX>=865-10&&bX<=932-70&&
        (red_diamond_flag1+red_diamond_flag2+ice_diamond_flag2+red_diamond_flag3+ice_diamond_flag3+red_diamond_flag4+ice_diamond_flag4==0))
    {WinFlash();Victory();return 1;}
    else return 0;
}

bool GameScene::judge_lose(double X,double Y){
    //方便debug，让其不死
    if(invincible)
        return 0;
    //    qDebug()<<"调用了判断失败函数";
    if(judge_water(X,Y)||judge_poison(X,Y))
    {
        qDebug()<<"失败了";
        Death->play();
        Fire->move(x,1000);
        ManfadeOutAnimation(FireCopy,X,Y);
        fadeOutAnimation(Cloud,X,Y);
        qDebug()<<"动画完成";
        Failure();
        return 1;
    }
    else return 0;
}

bool GameScene::judge_lose_ice(double X,double Y){
    //方便debug，让其不死
    if(invincible)
        return 0;
    //    qDebug()<<"调用了判断失败函数";
    if(judge_fire(X,Y)||judge_poison(X,Y))
    {
        qDebug()<<"失败了";
        Death->play();
        Ice->move(y,1000);
        ManfadeOutAnimation(IceCopy,X,Y);
        fadeOutAnimation(Cloud,X,Y);
        Failure();
        return 1;
    }
    else return 0;
}



void GameScene::timerEvent(QTimerEvent *time){
    if(time->timerId()==id1)
    {
        judge_lose(x,y);
        judge_win(x,y,bx,by);
        int modex= 0;
        int modey= 0;

        if(GetAsyncKeyState('A')&0x8000)modex--;
        if(GetAsyncKeyState('W')&0x8000)modey++;
        if(GetAsyncKeyState('D')&0x8000)modex++;

        if(modex==0)
            vx=0;

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
            { vy=vy_max;jump_fire->play();}
            else
            {
                vy=0;
                ay=0;
            }
        }
        if(judge_floor(x,y)==0)
        {
            if(judge_head(x,y)==1){
                vy=0;
            }
            if(vy<=vy_max&&vy>=-vy_max)
            {
                vy-=ay;
                ay=ay_max;
            }
            if(vy>vy_max)vy=vy_max;
            if(vy<-vy_max)vy=-vy_max;
        }

        dx+=vx;
        dy+=vy;
        x+=dx;
        y-=dy;

        //如果fireman跳到了地面上，强迫他在地面上，不能陷到地面里面
        if(judge_floor(x,y))
            y=adjust_floor (x,y);


        //可以在此处改变形态
        if(modex>0){
            Fire->setPixmap(*pixmapr);
        }
        else if(modex<0){
            Fire->setPixmap(*pixmapl);
        }
        else if(modex==0){
            Fire->setPixmap(*pixmap);
        }
        Fire->move(x,y);
        modex=0;
        modey=0;
        Fire->show();

        //以下是冰娃部分
        judge_lose_ice(bx,by);
        int modebx= 0;
        int modeby= 0;

        if(GetAsyncKeyState('J')&0x8000)modebx--;
        if(GetAsyncKeyState('I')&0x8000)modeby++;
        if(GetAsyncKeyState('L')&0x8000)modebx++;

        if(modebx==0)
        {
            bvx=0;
        }
        if(modebx==-1){
            if(judge_leftwall(bx,by)==1){
                bvx=0;
            }
            else{
                if(bvx>=-vx_max){
                    bvx-=bax/2;
                }
                else{
                    bvx=-vx_max;
                }
            }
        }
        if(modebx==1){
            if(judge_rightwall(bx,by)==1){
                bvx=0;
            }
            else{
                if(bvx<=vx_max){
                    bvx+=bax/2;
                }
                else{
                    bvx=vx_max;
                }
            }
        }

        if(judge_floor(bx,by))
        {
            if(modeby==1)
            { bvy=vy_max;jump_ice->play();}
            else
            {
                bvy=0;
                bay=0;
            }
        }
        if(judge_floor(bx,by)==0)
        {
            if(judge_head(bx,by)==1){
                bvy=0;
            }
            if(bvy<=vy_max&&bvy>=-vy_max)
            {
                bvy-=bay;
                bay=ay_max;
            }
            if(bvy>vy_max)bvy=vy_max;
            if(bvy<-vy_max)bvy=-vy_max;
        }

        bdx+=bvx;
        bdy+=bvy;
        bx+=bdx;
        by-=bdy;

        //如果fireman跳到了地面上，强迫他在地面上，不能陷到地面里面
        if(judge_floor(bx,by))
            by=adjust_floor (bx,by);


        //可以在此处改变形态
        Ice->setPixmap(*pixmap);
        if(modebx>0){
            Ice->setPixmap(*bpixmapr);
        }
        else if(modebx<0){
            Ice->setPixmap(*bpixmapl);
        }
        else if(modebx==0){
            Ice->setPixmap(*bpixmap);
        }
        Ice->move(bx,by);
        modebx=0;
        modeby=0;
        Ice->show();

        //检查黄色机关按钮是否被搬动
        if(y>=509-Height&&y<=530-Height&&x>=290-18&&x<=300-18&&dx<0&&yellowbuttonflag)
        {
            yOrgan->down();
            ySwitch->rightandleft(yellowbuttonflag);
            yellowbuttonflag=false;
        }
        if(y>=509-Height&&y<=530-Height&&x>=250-18&&x<=260-18&&dx>0&&!yellowbuttonflag)
        {
            yOrgan->up();
            ySwitch->rightandleft(yellowbuttonflag);
            yellowbuttonflag=true;
        }
        if(by>=509-Height&&by<=530-Height&&bx>=290-18&&bx<=300-18&&bdx<0&&yellowbuttonflag)
        {
            yOrgan->down();
            ySwitch->rightandleft(yellowbuttonflag);
            yellowbuttonflag=false;
        }
        if(by>=509-Height&&by<=530-Height&&bx>=250-18&&bx<=260-18&&bdx>0&&!yellowbuttonflag)
        {
            yOrgan->up();
            ySwitch->rightandleft(yellowbuttonflag);
            yellowbuttonflag=true;
        }

        //检查两个紫色开关是否有被按下的
        //第一个紫色开关
        if(y>=385-Height&&y<=400-Height&&x>=270-37&&x<=310-37&&purplebuttonflag1!=1&&purplebuttonflag1!=3){
            qDebug()<<"yes";
            if(purplebuttonflag1==0)
            {
                pOrgan->down();
                pSwitch1->upanddown(true);
            }
            if(purplebuttonflag1!=2)
                purplebuttonflag1=1;
            else
                purplebuttonflag1=3;
            purplebuttonflag=false;
        }
        else if(!(y>=385-Height&&y<=400-Height&&x>=270-37&&x<=310-37)&&(purplebuttonflag1==1||purplebuttonflag1==3)){
            qDebug()<<"not";
            if(purplebuttonflag1==3)
                purplebuttonflag1=2;
            else
                purplebuttonflag1=0;

            purplebuttonflag=!purplebuttonflag1&&!purplebuttonflag2;
            if(purplebuttonflag)
            {
                pOrgan->up();
                pSwitch1->upanddown(false);
            }
        }
        if(by>=385-Height&&by<=400-Height&&bx>=270-37&&bx<=310-37&&purplebuttonflag1!=2&&purplebuttonflag1!=3){
            qDebug()<<"yes";
            if(purplebuttonflag1==0)
            {
                pOrgan->down();
                pSwitch1->upanddown(true);
            }
            if(purplebuttonflag1!=1)
                purplebuttonflag1=2;
            else
                purplebuttonflag1=3;
            purplebuttonflag=false;
        }
        else if(!(by>=385-Height&&by<=400-Height&&bx>=270-37&&bx<=310-37)&&(purplebuttonflag1==2||purplebuttonflag1==3)){
            qDebug()<<"not";
            if(purplebuttonflag1==3)
                purplebuttonflag1=1;
            else
                purplebuttonflag1=0;

            purplebuttonflag=!purplebuttonflag1&&!purplebuttonflag2;
            if(purplebuttonflag)
            {
                pOrgan->up();
                pSwitch1->upanddown(false);
            }
        }



        //第二个紫色开关
        if(y>=286-Height&&y<=311-Height&&x>=740-37&&x<=780-37&&purplebuttonflag2!=1&&purplebuttonflag2!=3){
            qDebug()<<"yes";
            if(purplebuttonflag2==0)
            {
                pOrgan->down();
                pSwitch2->upanddown(true);
            }
            if(purplebuttonflag2!=2)
                purplebuttonflag2=1;
            else
                purplebuttonflag2=3;
            purplebuttonflag=false;
        }
        else if(!(y>=286-Height&&y<=311-Height&&x>=740-37&&x<=780-37)&&(purplebuttonflag2==1||purplebuttonflag2==3)){
            qDebug()<<"not";
            if(purplebuttonflag2==3)
                purplebuttonflag2=2;
            else
                purplebuttonflag2=0;

            purplebuttonflag=!purplebuttonflag1&&!purplebuttonflag2;
            if(purplebuttonflag)
            {
                pOrgan->up();
                pSwitch2->upanddown(false);
            }
        }
        if(by>=286-Height&&by<=311-Height&&bx>=740-37&&bx<=780-37&&purplebuttonflag2!=2&&purplebuttonflag2!=3){
            qDebug()<<"yes";
            if(purplebuttonflag2==0)
            {
                pOrgan->down();
                pSwitch2->upanddown(true);
            }
            if(purplebuttonflag2!=1)
                purplebuttonflag2=2;
            else
                purplebuttonflag2=3;
            purplebuttonflag=false;
        }
        else if(!(by>=286-Height&&by<=311-Height&&bx>=740-37&&bx<=780-37)&&(purplebuttonflag2==2||purplebuttonflag2==3)){
            qDebug()<<"not";
            if(purplebuttonflag2==3)
                purplebuttonflag2=1;
            else
                purplebuttonflag2=0;

            purplebuttonflag=!purplebuttonflag1&&!purplebuttonflag2;
            if(purplebuttonflag)
            {
                pOrgan->up();
                pSwitch2->upanddown(false);
            }
        }



        yOrgan->move(38,yOrgan->hei);
        yOrgan->show();
        pOrgan->move(864,pOrgan->hei);
        pOrgan->show();

        //火娃吃红色水晶
        if(x>=719-10-37&&x<=719+10-37&&y>=653-40-29&&y<=653+40-29)
        {
            if(red_diamond_flag1)
                Diamond->play();
            red_diamond_flag1=0;
        }
        if(red_diamond_flag1)
        {
            red_diamond1->setPixmap(*red_pixmap1);
            red_diamond1->move(719-28,653-21);//第一层水潭
            red_diamond1->show();
        }
        else
            red_diamond1->move(-100,-100);

        if(x>=674-10-37&&x<=674+10-37&&y>=500-40-29&&y<=500+40-29)
        {
            if(red_diamond_flag2)
                Diamond->play();
            red_diamond_flag2=0;
        }
        if(red_diamond_flag2)
        {
            red_diamond2->setPixmap(*red_pixmap2);
            red_diamond2->move(674-28,500-21);//第二层毒
            red_diamond2->show();
        }
        else
            red_diamond2->move(-100,-100);

        if(x>=200-10-37&&x<=200+10-37&&y>=352-40-29&&y<=352+40-29)
        {
            if(red_diamond_flag3)
                Diamond->play();
            red_diamond_flag3=0;
        }
        if(red_diamond_flag3)
        {
            red_diamond3->setPixmap(*red_pixmap3);
            red_diamond3->move(200-28,352-21);//第三层开关
            red_diamond3->show();
        }
        else
            red_diamond3->move(-100,-100);

        if(x>=448-10-37&&x<=448+10-37&&y>=133-40-29&&y<=133+40-29)
        {
            if(red_diamond_flag4)
                Diamond->play();
            red_diamond_flag4=0;

        }
        if(red_diamond_flag4)
        {
            red_diamond4->setPixmap(*red_pixmap4);
            red_diamond4->move(448-28,133-21);//第四层
            red_diamond4->show();
        }
        else
            red_diamond4->move(-100,-100);



        //冰娃吃蓝色水晶
        if(bx>=515-10-37&&bx<=515+10-37&&by>=653-40-29&&by<=653+40-29)
        {
            if(ice_diamond_flag1)
                Diamond->play();
            ice_diamond_flag1=0;
        }
        if(ice_diamond_flag1)
        {
            ice_diamond1->setPixmap(*ice_pixmap1);
            ice_diamond1->move(515-28,653-21);//一层火潭
            ice_diamond1->show();
        }
        else
            ice_diamond1->hide();
        if(bx>=617-10-37&&bx<=617+10-37&&by>=370-40-29&&by<=370+40-29)
        {
            if(ice_diamond_flag2)
                Diamond->play();
            ice_diamond_flag2=0;
        }
        if(ice_diamond_flag2)
        {
            ice_diamond2->setPixmap(*ice_pixmap2);
            ice_diamond2->move(617-28,370-21);//三层下坡
            ice_diamond2->show();
        }
        else
            ice_diamond2->move(-100,-100);

        if(bx>=67-10-37&&bx<=67+10-37&&by>=156-40-29&&by<=156+40-29)
        {
            if(ice_diamond_flag3)
                Diamond->play();
            ice_diamond_flag3=0;
        }
        if(ice_diamond_flag3)
        {
            ice_diamond3->setPixmap(*ice_pixmap3);
            ice_diamond3->move(67-28,156-21);//四层左
            ice_diamond3->show();
        }
        else
            ice_diamond3->move(-100,-100);

        if(bx>=553-10-37&&bx<=553+10-37&&by>=133-40-29&&by<=133+40-29)
        {ice_diamond_flag4=0;Diamond->play();}
        if(ice_diamond_flag4)
        {
            ice_diamond4->setPixmap(*ice_pixmap4);
            ice_diamond4->move(553-28,133-21);//四层右
            ice_diamond4->show();
        }
        else
            ice_diamond4->move(-100,-100);



        //box部分
        if((x>=boxx+60-25-5&&x<=boxx+60-25+5&&y<=282-Height+30&&y>=282-45-Height)/*右边*/
            ||(x>=boxx-Width+23-5&&x<=boxx-Width+23+5&&y<=282-Height+30&&y>=282-57-Height)/*左边*/)
        {
            boxdx=dx;
            //            Failure();
        }
        if((bx>=boxx+60-25-5&&bx<=boxx+60-25+5&&by<=282-Height+30&&by>=282-45-Height)/*右边*/
            ||(bx>=boxx-Width+23-5&&bx<=boxx-Width+23+5&&by<=282-Height+30&&by>=282-57-Height)/*左边*/)
        {
            boxdx=bdx;
            //            Failure();
        }
        boxx+=boxdx;


        box->setPixmap(*box_pixmap);
        box->move(boxx,boxy);
        box->show();
        boxdx=0;


        dx=0,dy=0;
        bdx=0,bdy=0;
    }
};

//背景图片的加载
void GameScene::paintEvent(QPaintEvent*){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/gamescene.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

}

//成功函数
void GameScene::Victory(){
    Win->show();

    // 定义一个局部变量，用于存储鼠标点击的区域
    int clickedArea = 0;

    // 创建一个临时的事件循环
    QEventLoop loop;

    // 连接鼠标点击信号到槽函数，以便在点击事件发生时退出事件循环
    connect(this, &GameScene::leftMouseButtonClicked, &loop, [&]() {
        if (isLeftClicked == 2)
        {
            clickedArea = isLeftClicked;
            loop.quit();
        }
    });

    // 开始事件循环，此时程序将阻塞，直到鼠标点击指定区域
    loop.exec();

    // 根据鼠标点击的区域执行相应的操作
    if (clickedArea == 2)
    {
        init();
        QTimer::singleShot(200, this, [=]() {
            // 告诉主场景，我返回了，主场景监听gameScene的返回按钮
            emit this->gameSceneBack();

        });
    }

    // 重置 isLeftClicked 变量
    isLeftClicked = 0;

    // 断开槽函数连接
    disconnect(this, &GameScene::leftMouseButtonClicked, nullptr, nullptr);
    connect(this, &GameScene::leftMouseButtonClicked, this, &GameScene::handleMouseClick);
}

void GameScene::handleMouseClick()
{
    if (QApplication::mouseButtons() == Qt::LeftButton)
    {
        // 获取鼠标点击位置的矩形区域
        QRect targetRect1(380, 325, 230, 35);
        QRect targetRect2(425, 410, 145, 75);

        // 获取鼠标点击位置
        QPoint mousePos = mapFromGlobal(QCursor::pos());

        // 检查是否点击在矩形区域内
        if (targetRect1.contains(mousePos))
        {
            isLeftClicked = 1;
        }
        else if (targetRect2.contains(mousePos))
        {
            isLeftClicked = 2;
        }
    }
}

void GameScene::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isLeftClicked = 0;
        emit leftMouseButtonClicked();
    }
    QMainWindow::mousePressEvent(event);
}


//失败函数
void GameScene::Failure(){

    Fail->show();

    // 定义一个局部变量，用于存储鼠标点击的区域
    int clickedArea = 0;

    // 创建一个临时的事件循环
    QEventLoop loop;

    // 连接鼠标点击信号到槽函数，以便在点击事件发生时退出事件循环
    connect(this, &GameScene::leftMouseButtonClicked, &loop, [&]() {
        if (isLeftClicked == 1 || isLeftClicked == 2)
        {
            clickedArea = isLeftClicked;
            loop.quit();
        }
    });

    // 开始事件循环，此时程序将阻塞，直到鼠标点击指定区域
    loop.exec();

    // 根据鼠标点击的区域执行相应的操作
    if (clickedArea == 1)
    {
        init();
    }
    else if (clickedArea == 2)
    {
        init();
        QTimer::singleShot(200, this, [=]() {
            // 告诉主场景，我返回了，主场景监听gameScene的返回按钮
            emit this->gameSceneBack();
        });
    }

    // 重置 isLeftClicked 变量
    isLeftClicked = 0;

    // 断开槽函数连接
    disconnect(this, &GameScene::leftMouseButtonClicked, nullptr, nullptr);
    connect(this, &GameScene::leftMouseButtonClicked, this, &GameScene::handleMouseClick);

}

//初始化函数
void GameScene::init(){
    x=ini_x;
    y=ini_y;
    Fire->move(x,y);
    bx=b_ini_x;
    by=b_ini_y;
    Ice->move(bx,by);
    red_diamond_flag1=1;
    ice_diamond_flag1=1;
    red_diamond_flag2=1;
    ice_diamond_flag2=1;
    red_diamond_flag3=1;
    ice_diamond_flag3=1;
    red_diamond_flag4=1;
    ice_diamond_flag4=1;

    boxx=box_ini_x;
    boxy=box_ini_y;
    box->move(boxx,boxy);
    yOrgan->hei=yOrgan->ini_hei;
    pOrgan->hei=pOrgan->ini_hei;
    ySwitch->init();
    yellowbuttonflag=true;
    //初始化需要增加的量
    FireDoor->move(801,100);
    IceDoor->move(876,100);
    FireBack->move(806,103);
    IceBack->move(880,103);
    FireBack->hide();
    IceBack->hide();
    Fail->hide();
    isLeftClicked = 0;
    //——改动
    Win->hide();
}


