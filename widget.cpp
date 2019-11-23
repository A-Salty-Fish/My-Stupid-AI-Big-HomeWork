#include "widget.h"
#include "ui_widget.h"
#include "qpainter.h"
#include "ants.h"
#include "gene.h"
#include"qtimer.h"
#include"windows.h"

static MyMaze m;
static int maze[(MAXLENGTH)*MAXLENGTH];
static bool Genetated=0;
static int BOXWIDTH=40;//一格的宽度 正方形
static int DRAWWIDTH=MAXLENGTH*BOXWIDTH;//画布长度

const int ANTSNUM=5;
static Ant ants[ANTSNUM];
static int min;
static int PheRed=125/ANTSNUM;
static bool ShowAntWay=0;
static QTimer *timer=nullptr;
static bool isAnt;

static Gene group[GeneNum];
static bool FirstTurn=true;
static QColor GeneColor[MAXLENGTH][MAXLENGTH];
static bool ShowGeneWay=0;

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

    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
}
void Widget::paintEvent(QPaintEvent *event){//绘图函数
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
                        painter.fillRect(ThisRect,GeneColor[i][j]);
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
    }
}
Widget::~Widget()
{
    delete ui;
}

void Widget::on_PushButton_clicked()
{
    if (Genetated==0){
        min=MAXNUMBER*MAXLENGTH;
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
        ui->GenMinButton_2->setEnabled(true);
    }
}

void Widget::on_ResetButton_clicked()
{
    if (timer!=nullptr) timer->stop();
    if (Genetated==1){
        Genetated=0;
        for (int i=0;i<MAXLENGTH;i++)
            for (int j=0;j<MAXLENGTH;j++)
                maze[i*MAXLENGTH+j]=ROUTE;
        min=MAXLENGTH*MAXNUMBER;
        ui->AntText->clear();
        update();
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
        if (ants[i].Length()<min){ min=ants[i].Length();mini=i;is_smaller=true;}
    }
    if(is_smaller) ants[mini].UpdateMinStack();//更新保存最短路径的栈
    for (int i=0;i<ANTSNUM;i++){
        ants[i].RemainPheromone(maze);
    }
    Ant::UpdatePheromone(maze);
    update();
    QString str=QString::number(min,10);
    ui->AntText->setText(str);
}

void Widget::on_AutoButton_clicked()
{
    ShowAntWay=0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_ContinueButton_clicked()));
    timer->start(300);
}

void Widget::on_AntMinButton_clicked()
{
    ui->AntMinButton->setEnabled(false);
    if (timer!=nullptr)
        timer->stop();
    ShowAntWay=1;
    update();

}

void Widget::on_ContinueButton_2_clicked()
{
    isAnt=false;
    if (FirstTurn) {
        for (int i=0;i<GeneNum;i++){
            group[i]=Gene();
            group[i].Initial();
        }
        FirstTurn=false;
    }
    else{
        Gene::NewTurn(group,maze);
    }
    update();
}

void Widget::on_GenMinButton_2_clicked()
{
    ui->GenText->clear();
    Gene g;
    char c[34];
    g.Initial();
    g.GeneToChar(c);
    QString str = QString(c);
    //    QString str =QString::number(g.onenum(),10);
    ui->GenText->setText(str);
    g.Variation(1);
    g.GeneToChar(c);
//    QString str2=QString::number(g.Length(maze));
    str=QString(c);
    ui->AntText->setText(str);
}


