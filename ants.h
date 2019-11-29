#ifndef ANTS_H
#define ANTS_H

#endif // ANTS_H
#include"maze.h"
#include"qstack.h"

const double alpha=0.7;//挥发常数
static const double MinPhe=1.0000005;//每格最小的信息素浓度
static double Pheromone[MAXLENGTH][MAXLENGTH];//保存信息素的数组
static QStack<int> min_x,min_y;//保存最优解
const int MAXWAY=(2*MAXLENGTH-4)*MAXNUMBER;//可能的最长距离

static const int ANTSNUM=10;//一组蚂蚁的数量
const double PheRate=(double)4/ANTSNUM;//每只蚂蚁每次留下信息素浓度的比例常数
static int MinOfAnt=MAXLENGTH*MAXNUMBER;//存储最优解的值
static int PheRed=60/ANTSNUM;//用来绘图的颜色常数 越红代表信息素浓度越高


int SelectDirection(double right,double down){//轮盘赌法 以右方的信息素浓度与下方信息素浓度的比值随机选择一个方向
    const int total=500;
    int rightp=(int)((double)total*right/(right+down));
    qsrand((uint)QTime::currentTime().msec());
    int randnum=qrand()%total;
    if (randnum<rightp) return RIGHT;
    else return DOWN;
}

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
    static void UpdatePheromone(){//每轮后信息素浓度衰减
        for (int i=1;i<MAXLENGTH-1;i++)
            for (int j=1;j<MAXLENGTH-1;j++){
                Pheromone[i][j]=alpha*(Pheromone[i][j]-MinPhe)+1;
            }
    }
    void RemainPheromone(){//在走过的路径上留下信息素
        while(!back_x.empty()&&!back_y.empty()){
            int px=back_x.pop();int py=back_y.pop();
            Pheromone[px][py]+=PheRate*MAXWAY/length;//距离越短 信息素浓度越大
        }
    }
    void FindWay(int * maze){//每只蚂蚁每一步的寻路
        //如果到终点 则停止
        if (x==MAXLENGTH-2&&y==MAXLENGTH-2&&back==0){ back=1;back_x.pop();back_y.pop();}
        if (back==1){return;}
        //如果没到终点
        else if (back==0){
            int direction;
            direction=SelectDirection(Pheromone[x][y+1],Pheromone[x+1][y]);//信息素浓度越大的方向越容易被选中
            //按选中的方向更新坐标
            if (direction==RIGHT&&y==MAXLENGTH-2) direction=DOWN;
            else if (direction==DOWN&&x==MAXLENGTH-2) direction=RIGHT;
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
                Sleep(qrand()%10);
                ants[i].FindWay(maze);
            }
        int mini=0;//这轮蚁群中距离最短的蚂蚁编号
        bool is_smaller=false;//是否比之前的最优解更好
        for (int i=0;i<ANTSNUM;i++){//找到最短的那只
            if (ants[i].Length()<MinOfAnt){ MinOfAnt=ants[i].Length();mini=i;is_smaller=true;}
        }
        if(is_smaller) ants[mini].UpdateMinStack();//更新保存最短路径的栈
        for (int i=0;i<ANTSNUM;i++){
            ants[i].RemainPheromone();//所有蚂蚁在路径上留下信息素
        }
        UpdatePheromone();//更新地图上的信息素
    }

};
