#include "widget.h"
#include "ui_widget.h"
#include "qpainter.h"
#include "ants.h"
#include "gene.h"
#include"qtimer.h"


static MyMaze m;
static int maze[(MAXLENGTH)*MAXLENGTH];
static bool Genetated=0;
static int BOXWIDTH=40;//一格的宽度 正方形
static int DRAWWIDTH=MAXLENGTH*BOXWIDTH;//画布长度

static bool ShowDPWay=false;

const int ANTSNUM=5;
static Ant ants[ANTSNUM];
static int MinOfAnt=MAXLENGTH*MAXNUMBER;
static int PheRed=125/ANTSNUM;
static bool ShowAntWay=0;
static QTimer *AntTimer=nullptr;
static bool isAnt;

static Gene group[GeneNum];
static bool FirstTurn=true;
static int GeneColor[MAXLENGTH*MAXLENGTH];
static int MinOfGene=MAXLENGTH*MAXNUMBER;
static Gene BestGene;
static bool ShowGeneWay=0;
static QTimer *GeneTimer=nullptr;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->ResetButton->setEnabled(false);
    ui->AutoButton->setEnabled(false);
    ui->AntMinButton->setEnabled(false);
    ui->ContinueButton->setEnabled(false);
    ui->ContinueButton_2->setEnabled(false);
    ui->AutoButton_2->setEnabled(false);
    ui->GenMinButton_2->setEnabled(false);
    ui->DPButton->setEnabled(false);
    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
}
void Widget::paintEvent(QPaintEvent *event){    //绘图函数
    QPainter painter(this  );
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    QRect rect(0,0,DRAWWIDTH,DRAWWIDTH);
    QPen pen;
    pen.setWidth(3);
    pen.setColor(Qt::black);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::BevelJoin);
    painter.setPen(pen);
    QBrush brush;
    brush.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    if (Genetated==1){
        for(int i=0;i<MAXLENGTH;i++)
            for (int j=0;j<MAXLENGTH;j++){
                QBrush brush;
                QRect ThisRect(j*BOXWIDTH,i*BOXWIDTH,BOXWIDTH,BOXWIDTH);
                switch (maze[i*MAXLENGTH+j]) {
                case WALL:
                    brush.setColor(Qt::blue);    painter.setBrush(brush);
                    painter.fillRect(ThisRect,Qt::blue);break;
                default:
                    painter.fillRect(ThisRect,QColor(5,0,0,255));
                    QString str=QString::number(maze[i*MAXLENGTH+j],10);
                    QPen pen; pen.setWidth(4); pen.setColor(Qt::green);painter.setPen(pen);
                    if (ShowAntWay==0&&isAnt){
                        if (Pheromone[i][j]>MinPhe+0.00005){
                            int Red=(int)(Pheromone[i][j]*PheRed);
                            Red=Red>=255?255:Red;
                            Red=Red>=0?Red:0;
                            painter.fillRect(ThisRect,QColor(Red,0,0,255));
                        }
                    }
                    if (ShowGeneWay==0&&!isAnt){
                        painter.fillRect(ThisRect,QColor(5,0,GeneColor[i*MAXLENGTH+j]>255?255:GeneColor[i*MAXLENGTH+j]));
                    }
                    painter.drawText(j*BOXWIDTH+BOXWIDTH/3,i*BOXWIDTH+BOXWIDTH/2,str);
                }
            }
        if (ShowAntWay==1){
            QStack<int> tmpx,tmpy;
            while (!min_x.empty()&&!min_y.empty()) {
                brush.setColor(Qt::white);    painter.setBrush(brush);
                QRect ThisRect(min_y.top()*BOXWIDTH,min_x.top()*BOXWIDTH,BOXWIDTH,BOXWIDTH);
                painter.fillRect(ThisRect,Qt::white);
                QString str=QString::number(maze[min_x.top()*MAXLENGTH+min_y.top()],10);
                painter.drawText(min_y.top()*BOXWIDTH+BOXWIDTH/3,min_x.top()*BOXWIDTH+BOXWIDTH/2,str);
                tmpx.push(min_x.pop());tmpy.push(min_y.pop());
            }
            while(!tmpx.empty()&&!tmpy.empty()){
                min_x.push(tmpx.pop());min_y.push(tmpy.pop());
            }
        }
        if (ShowGeneWay==1){
            int x=1,y=1;
            for (int i=0;i<GeneLength-1;i++){
                if (BestGene.GetBit(i)==1)x++;
                else y++;
                QRect ThisRect(y*BOXWIDTH,x*BOXWIDTH,BOXWIDTH,BOXWIDTH);
                painter.fillRect(ThisRect,Qt::white);
                QString str=QString::number(maze[x*MAXLENGTH+y],10);
                painter.drawText(y*BOXWIDTH+BOXWIDTH/3,x*BOXWIDTH+BOXWIDTH/2,str);
            }
        }
        if (ShowDPWay==1){
            for (int i=1;i<MAXLENGTH-1;i++){
                for (int j=1;j<MAXLENGTH-1;j++){
                    if (DPPath[i][j]==1){
                        QRect ThisRect(j*BOXWIDTH,i*BOXWIDTH,BOXWIDTH,BOXWIDTH);
                        painter.fillRect(ThisRect,Qt::yellow);
                        QString str=QString::number(maze[i*MAXLENGTH+j],10);
                        painter.drawText(j*BOXWIDTH+BOXWIDTH/3,i*BOXWIDTH+BOXWIDTH/2,str);
                    }
                }
            }
        }
    }
}
Widget::~Widget()
{
    delete ui;
}

void Widget::on_PushButton_clicked()
{
    if (Genetated==0){
        MinOfAnt=MAXNUMBER*MAXLENGTH;
        isAnt=true;
        m=MyMaze();
        m.CreateMazeWithWeights();
        m.MazeToAry(maze);
        for (int i=0;i<MAXLENGTH;i++)
            for (int j=0;j<MAXLENGTH;j++)
            {
                Pheromone[i][j]=MinPhe;
            }
        for (int i=0;i<ANTSNUM;i++) ants[i]= Ant();
        for (int i=0;i<MAXLENGTH;i++) for (int j=0;j<MAXLENGTH;j++) GeneColor[i*MAXLENGTH+j]=0;
        Genetated=1;
        update();
        QString dpstr=QString::number(m.DP(),10);
        ui->DPLable->setText(dpstr);
        ui->PushButton->setEnabled(false);
        ui->ResetButton->setEnabled(true);
        ui->AutoButton->setEnabled(true);
        ui->ContinueButton->setEnabled(true);
        ui->ContinueButton_2->setEnabled(true);
        ui->AutoButton_2->setEnabled(true);
        ui->DPButton->setEnabled(true);
    }
}

void Widget::on_ResetButton_clicked()
{
    if (AntTimer!=nullptr) AntTimer->stop();
    if (GeneTimer!=nullptr) GeneTimer->stop();
    if (Genetated==1){
        Genetated=0;
        ShowAntWay=0;
        ShowDPWay=0;
        ShowGeneWay=0;
        isAnt=true;
        FirstTurn=true;
        for (int i=0;i<MAXLENGTH;i++)
            for (int j=0;j<MAXLENGTH;j++)
                maze[i*MAXLENGTH+j]=ROUTE;
        MinOfAnt=MAXLENGTH*MAXNUMBER;
        MinOfGene=MAXLENGTH*MAXNUMBER;
        ui->AntText->clear();
        update();
        ui->GenText->clear();
        ui->GeneMinText->clear();
        ui->PushButton->setEnabled(true);
        ui->AutoButton->setEnabled(false);
        ui->AntMinButton->setEnabled(false);
        ui->ContinueButton->setEnabled(false);
    }
}


void Widget::on_ContinueButton_clicked()
{
    ShowAntWay=0;
    ui->AntMinButton->setEnabled(true);
    ui->ContinueButton_2->setEnabled(false);
    ui->AutoButton_2->setEnabled(false);
    ui->GenMinButton_2->setEnabled(false);
    for (int i=0;i<ANTSNUM;i++) ants[i]= Ant();
    for (int n=0;n<2*MAXLENGTH-4;n++)
        for (int i=0;i<ANTSNUM;i++){
            qsrand((uint)QTime::currentTime().msec());
            Sleep(qrand()%20);
            ants[i].FindWay(maze);
        }
    int mini=0;
    bool is_smaller=false;
    for (int i=0;i<ANTSNUM;i++){//找到最短的那只
        if (ants[i].Length()<MinOfAnt){ MinOfAnt=ants[i].Length();mini=i;is_smaller=true;}
    }
    if(is_smaller) ants[mini].UpdateMinStack();//更新保存最短路径的栈
    for (int i=0;i<ANTSNUM;i++){
        ants[i].RemainPheromone(maze);
    }
    Ant::UpdatePheromone(maze);
    update();
    QString str=QString::number(MinOfAnt,10);
    ui->AntText->setText(str);
}

void Widget::on_AutoButton_clicked()
{
    ShowAntWay=0;
    AntTimer = new QTimer(this);
    connect(AntTimer, SIGNAL(timeout()), this, SLOT(on_ContinueButton_clicked()));
    AntTimer->start(300);
}

void Widget::on_AntMinButton_clicked()
{
    ui->AntMinButton->setEnabled(false);
    if (AntTimer!=nullptr)
        AntTimer->stop();
    ShowAntWay=1;
    update();

}

void Widget::on_ContinueButton_2_clicked()
{
    ShowGeneWay=0;
    ui->AutoButton->setEnabled(false);
    ui->AntMinButton->setEnabled(false);
    ui->ContinueButton->setEnabled(false);
    ui->GenMinButton_2->setEnabled(true);
    char CharGen[GeneLength+2];
    isAnt=false;
    if (FirstTurn) {
        int i;
        int min=INF;
        int mini=0;
        for (i=0;i<GeneNum;i++){
            group[i]=Gene();
            qsrand((uint)QTime::currentTime().msec());
            Sleep(qrand()%10);
            group[i].Initial();
            group[i].Variation(2);
            int l=group[i].Length(maze);
            if (l<min){
                min=l;
                mini=i;
            }
        }
        MinOfGene=min;
        BestGene=group[mini];
        ui->GeneMinText->setText(QString::number(min,10));
        group[mini].GeneToChar(CharGen);
        ui->GenText->setText(QString(CharGen));
        FirstTurn=false;
        Gene::UpdateQColor(group,GeneColor);
    }
    else{
        Gene::NewTurn(group,maze);
        if (group[0].ReturnLength()<BestGene.ReturnLength()){
            BestGene=group[0];
        }
        ui->GeneMinText->setText(QString::number(BestGene.ReturnLength(),10));
        BestGene.GeneToChar(CharGen);
         ui->GenText->setText(QString(CharGen));
        Gene::UpdateQColor(group,GeneColor);
    }

    update();
}


void Widget::on_AutoButton_2_clicked()
{
    ShowGeneWay=0;
    GeneTimer = new QTimer(this);
    connect(GeneTimer, SIGNAL(timeout()), this, SLOT(on_ContinueButton_2_clicked()));
    GeneTimer->start(300);
}

void Widget::on_GenMinButton_2_clicked()
{
    ui->GenMinButton_2->setEnabled(false);
    if (GeneTimer!=nullptr)
        GeneTimer->stop();
    ShowGeneWay=1;
    update();
}


void Widget::on_DPButton_clicked()
{
    ShowDPWay=true;
    ui->DPButton->setEnabled(false);
    update();
}
