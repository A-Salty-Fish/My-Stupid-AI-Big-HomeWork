#ifndef ANTS_H
#define ANTS_H

#include"maze.h"
#include<QStack>
#include<QThread>

const double beta=0.7;//挥发常数
static const double MinPhe=1.0000005;//每格最小的信息素浓度
static double Pheromone[MAXLENGTH][MAXLENGTH];//保存信息素的数组

static QStack<int> min_x,min_y;//保存最优解
static int MinOfAnt=MAXLENGTH*MAXNUMBER;//存储最优解的值

const int MAXWAY=(2*MAXLENGTH-4)*MAXNUMBER;//可能的最长距离

static const int ANTSNUM=5;//一组蚂蚁的数量
const double PheRate=(double)8/ANTSNUM;//每只蚂蚁每次留下信息素浓度的比例常数

static int PheRed=100/ANTSNUM;//用来绘图的颜色常数 越红代表信息素浓度越高

class Ant{
    int x, y;//坐标
    int length;//已经走过的距离
    bool back;//是否到达
public:
    QStack<int> back_x,back_y;//存储路径
    Ant():x(1),y(1),length(0),back(0){}
    int Length(){return length;}

    void UpdateMinStack(){//更新保存最优解的栈
        QStack<int> tmpx,tmpy;
        while (!min_x.empty()&&!min_y.empty()){
            min_x.pop();min_y.pop();
        }
        while(!back_x.empty()&&!back_y.empty()){
            tmpx.push(back_x.pop());tmpy.push(back_y.pop());
        }
        while(!tmpx.empty()&&!tmpy.empty()){
            min_x.push(tmpx.top());back_x.push(tmpx.pop());
            min_y.push(tmpy.top());back_y.push(tmpy.pop());
        }
    }

    int SelectDirection(double right,double down,int *maze){//轮盘赌法 以右方的信息素浓度与下方信息素浓度的比值随机选择一个方向
        qsrand((uint)QTime::currentTime().msec());//随机种子
        //如果到达边界 则只能往一个方向走
        if (x==MAXLENGTH-2)
            return RIGHT;
        else if (y==MAXLENGTH-2)
            return DOWN;
        //否则要根据信息素浓度和启发值选择一个方向
        else{
            bool First=qrand()%2;//决定哪个方向放在首部
            const int total=1000;
            double ur=MAXNUMBER-maze[x*MAXLENGTH+y+1];//右方启发值
            double ud=MAXNUMBER-maze[(x+1)*MAXLENGTH+y];//下方启发值
            int rightp=(int)(total*(ur*right/(ur*right+ud*down)));//边界值 如果生成的随机数小于这个值 则选择某方向 大于等于则选择另一个方向
            Sleep(qrand()%3);//为了让种子更随机
            qsrand((uint)QTime::currentTime().msec());
            int randnum=qrand()%total;//生成随机数
            //返回方向
            if (First){
                if (randnum<rightp)
                    return RIGHT;
                else
                    return DOWN;
            }
            else{
                if (randnum<rightp)
                    return DOWN;
                else
                    return RIGHT;
            }
        }
    }
    static void UpdatePheromone(){//每轮后信息素浓度衰减
        for (int i=1;i<MAXLENGTH-1;i++)
            for (int j=1;j<MAXLENGTH-1;j++){
                Pheromone[i][j]=beta*(Pheromone[i][j]-MinPhe)+1;
            }
    }
    void RemainPheromone(int *maze){//在走过的路径上留下信息素  使用Ant-Circle System
        while(!back_x.empty()&&!back_y.empty()){
            int px=back_x.pop();int py=back_y.pop();
            Pheromone[px][py]+=PheRate*(((double)MAXWAY/ANTSNUM)/length);//距离越短 信息素浓度越大
        }
    }
    void FindWay(int * maze){//每只蚂蚁每一步的寻路
        //如果到终点 则停止
        if (x==MAXLENGTH-2&&y==MAXLENGTH-2&&back==0){
            back=1;
            back_x.pop();
            back_y.pop();
        }
        if (back==1){return;}
        //如果没到终点
        else if (back==0){
            int direction;
            direction=SelectDirection(Pheromone[x][y+1],Pheromone[x+1][y],maze);//选择方向
            //按选中的方向更新坐标
            switch (direction) {
            case RIGHT:y++; break;
            case DOWN:x++; break;
            }
            length+=maze[x*MAXLENGTH+y];//更新走过的距离
            back_x.push(x);back_y.push(y);//记录走过的路径
        }
    }
    static void NewTurn(Ant* ants,int * maze){//每轮所有蚂蚁的寻路
        for (int i=0;i<ANTSNUM;i++) ants[i]= Ant();//新建蚂蚁群
        //所有蚂蚁寻路
        for (int n=0;n<2*MAXLENGTH-4;n++)
            for (int i=0;i<ANTSNUM;i++){
                qsrand((uint)QTime::currentTime().msec());
                Sleep(qrand()%5);
                ants[i].FindWay(maze);
            }
        int mini=0;//这轮蚁群中距离最短的蚂蚁编号
        bool is_smaller=false;//是否比之前的最优解更好
        for (int i=0;i<ANTSNUM;i++){//找到最短的那只
            if (ants[i].Length()<MinOfAnt){
                MinOfAnt=ants[i].Length();
                mini=i;
                is_smaller=true;
            }
        }
        if(is_smaller) ants[mini].UpdateMinStack();//更新保存最短路径的栈
        for (int i=0;i<ANTSNUM;i++){
            ants[i].RemainPheromone(maze);//所有蚂蚁在路径上留下信息素
        }
        UpdatePheromone();//更新地图上的信息素
    }
};

#endif // ANTS_H
