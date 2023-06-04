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
        //延时返回到主场景
        QTimer::singleShot(500,this,[=](){
            //告诉主场景，我返回了，主场景监听gameScene的返回按钮
            emit this->gameSceneBack();
        });
    });


    //刷新率
    id1=startTimer(20);

    Fire = new QLabel(this);
    Ice=new QLabel(this);

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

    //固定fireman的大小
    Fire->setFixedSize(Width,Height);
    Ice->setFixedSize(Width,Height);
}


//同一块障碍物地板要高于墙一点，否则会卡bug  地板要比墙靠里一点点
bool judge_floor(double X,double Y)//判定地板
{
    if(Y>=708-height/*底层*/
        ||(Y>=606-height&&Y<=640-height&&X>=0-width1&&X<=340-width1)/*一层夹层墙*/
        ||(Y>=628-height&&X>882-width1)/*一层右边墙*/
        ||(Y>=509-height&&Y<=530-height&&X<=490-width1)/*二层高墙*/
        ||(Y>=465-height&&Y<=500-height&&X<=141-width1)/*黄色机关墙*/
        ||(Y>=557-height&&Y<=580-height&&X>=438-width1&&X<=830-width1)/*二层低墙*/
        ||((Y>=385-height)&&(Y<=400-height)&&X>=136-width1&&X<=562-width1)/*三层左墙*/
        ||((Y>=410-height)&&(Y<=430-height)&&X>=561-width1)/*三层右墙*/
        ||(Y>=313-height&&Y<=332-height&&X>=864-width1)/*紫色机关*/
        ||(Y>=286-height&&Y<=311-height&&X<=860-width1)||(Y>=247-height&&Y<=282-height&&X<=212-width1)||(Y>=186-height&&Y<=297-height&&X<=168-width1)/*四层三个墙*/
        ||(Y>=160-height&&Y<=185-height&&X>=254-width1)/*第四层*/
        )
        return 1;
    else
        return 0;
}
int adjust_floor(double X,double Y){
    if(Y>=708-height)        return 708-height;
    else if(Y>=606-height&&Y<=640-height&&X>=0-width1&&X<=340-width1)        return 606-height;
    else if(Y>=628-height&&X>882-width1)        return 628-height;
    else if(Y>=509-height&&Y<=530-height&&X<=490-width1)        return 509-height;
    else if(Y>=465-height&&Y<=500-height&&X<=141-width)        return 465-height;
    else if(Y>=557-height&&Y<=580-height&&X>=438-width1&&X<=830-width1)        return 557-height;
    else if((Y>=385-height)&&(Y<=400-height)&&X>=136-width1&&X<=562-width1)        return 385-height;
    else if((Y>=410-height)&&(Y<=430-height)&&X>=561-width1)        return 410-height;
    else if(Y>=313-height&&Y<=332-height&&X>=864-width1)      return 313-height;
    else if(Y>=286-height&&Y<=311-height&&X<=860-width1)        return 286-height;
    else if(Y>=247-height&&Y<=282-height&&X<=212-width1)       return 247-height;
    else if(Y>=186-height&&Y<=297-height&&X<=168-width1)        return 186-height;
    else if(Y>=160-height&&Y<=185-height&&X>=254-width1)        return 160-height;
    else return Y;
}
bool judge_leftwall(double X,double Y)//判定左墙
{
    if(X<=38-width/*左边全墙*/
        ||(X<=330&&X>=320&&Y>=608-height&&Y<=634-height)/*一层中间卡墙*/
        ||(Y>=560&&Y<=588&&X>=842-lww&&X<=842)/*二层右墙*/
        ||(Y>=512-50&&Y<=557-50&&X>=480-width-lww&&X<=480-width)/*二层中间墙*/
        ||(Y>=463-lwh&&Y<=510-lwh&&X<=120) /*二层机关右边*/
        ||(Y>=385-50&&Y<=430-50&&X>=560-width-lww&&X<=560-width)/*三层中间墙*/
        ||(Y>=243-50&&Y<=282-50&&X>=205-width-lww&&X<=205-width)/*箱子左边*/
        ||(Y>=288-50&&Y<=332-50&&X>=866-width-lww&&X<=866-width)/*紫色机关左墙*/
        ||(Y>=188-50&&Y<=241-50&&X>=166-width-lww&&X<=166-width)/*箱子上面的左墙*/
        )
        return 1;
    else
        return 0;
}
bool judge_rightwall(double X,double Y)//判定右墙
{
    if((X>=835&&X<=855&&Y>=580)/*一层右墙*/
        ||X>=950-40/*右侧墙*/
        )
        return 1;
    else
        return 0;
}
bool judge_head(int X,int Y)//判定头是否撞墙
{
    if((Y>=585&&Y<=610&&X>=0&&X<=330)/*？*/

        ||(Y>=150&&Y<=190&&X>=415)/*四层上墙*/
        ||(Y>=557-height1&&Y<=588-height1&&X>=409&&X<=830)/*一层上墙*/
        ||Y<=42/*上顶墙*/
        ||(Y>=160&&Y<=185&&X>=254)/**/
        ||(Y>=511-height1&&Y<=534-height1&&X>=0&&X<=423)/*一层夹层上墙*/
        ||(Y>=562-height1&&Y<=590-height1&&X>=441&&X<=830)/*一层上墙*/
        ||(Y>=379-height1&&Y<=414-height1&&X>=140-20&&X<=520)/*二层左墙*/
        ||(Y>=409-height1&&Y<=434-height1&&X>=520&&X<=962)/*二层右墙*/
        ||(Y>=260-height1&&Y<=318-height1&&X>=0&&X<=717)/*三层左墙*/
        ||(Y>=288-height1&&Y<=337-height1&&X>=713&&X<=861)/*三层右墙*/
        )
        return 1;
    else
        return 0;
}

bool judge_fire(double X,double Y)//岩浆判定
{
    if(Y>=708-height&&X>=470-width&&X<=580-width)
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


bool GameScene::judge_win(double X,double Y,double bX,double bY){
    if(X>=790&&X<=856&&Y>=160-58&&Y<=185-58&&bY>=160-58&&bY<=185-58&&bX>=865&&bX<=932)
    {Victory();return 1;}
     else return 0;
}

bool GameScene::judge_lose(double X,double Y){
//    qDebug()<<"调用了判断失败函数";
    if(judge_water(X,Y)||judge_poison(X,Y))
    {
        qDebug()<<"失败了";
        Fire->move(x,1000);
        Failure();
        return 1;
    }
    else return 0;
}

bool GameScene::judge_lose_ice(double X,double Y){
    //    qDebug()<<"调用了判断失败函数";
    if(judge_fire(X,Y)||judge_poison(X,Y))
    {
        qDebug()<<"失败了";
        Ice->move(y,1000);
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

//        if(dy!=0)
//            qDebug()<<x<<y;

        dx=0,dy=0;
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
                bvy=vy_max;
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

        bdx=0,bdy=0;
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
    int ret2=QMessageBox::information(this,"闯关成功","恭喜你闯关成功！",QMessageBox::Ok);
    if(ret2){
        //延时返回到主场景
        QTimer::singleShot(200,this,[=](){
            //告诉主场景，我返回了，主场景监听gameScene的返回按钮
            emit this->gameSceneBack();
        });
        x=ini_x;
        y=ini_y;
        Fire->move(x,y);
        bx=b_ini_x;
        by=b_ini_y;
        Ice->move(bx,by);
    }
}

//失败函数
void GameScene::Failure(){
    int ret1=QMessageBox::question(this,"闯关失败","是否重开？",QMessageBox::Yes,QMessageBox::No);
    if(ret1==QMessageBox::Yes){
//        qDebug()<<ini_x<<ini_y;
        x=ini_x;
        y=ini_y;
        Fire->move(x,y);
        Fire->show();
        bx=b_ini_x;
        by=b_ini_y;
        Ice->move(bx,by);
        Ice->show();
    }
    else{
        //将fireman设置为初始位置

        //延时返回到主场景
        QTimer::singleShot(200,this,[=](){
            //告诉主场景，我返回了，主场景监听gameScene的返回按钮
            emit this->gameSceneBack();
        });
        x=ini_x;
        y=ini_y;
        Fire->move(x,y);
        bx=b_ini_x;
        by=b_ini_y;
        Ice->move(bx,by);
    }
}
