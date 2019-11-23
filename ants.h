#ifndef ANTS_H
#define ANTS_H

#endif // ANTS_H
#include"maze.h"
#include"qstack.h"

const double alpha=0.8;//挥发常数
static const double MinPhe=1.0000005;
const double PheRate=0.002;
static double Pheromone[MAXLENGTH][MAXLENGTH];
static QStack<int> min_x,min_y;
const int MAXWAY=(2*MAXLENGTH-4)*MAXNUMBER;

int SelectDirection(double right,double down){//轮盘法
    const int total=500;
    int rightp=(int)((double)total*right/(right+down));
    qsrand((uint)QTime::currentTime().msec());
    int randnum=qrand()%total;
    if (randnum<rightp) return RIGHT;
    else return DOWN;
}

class Ant{
    int x, y;
    int length;
    bool back;
public:
    QStack<int> back_x,back_y;
    Ant():x(1),y(1),length(0),back(0){
    }

    void UpdateMinStack(){
        QStack<int> tmpx,tmpy;
        while (!min_x.empty()&&!min_y.empty()){//清空原来的栈
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
    static void UpdatePheromone(int* maze){
        for (int i=1;i<MAXLENGTH-1;i++)
            for (int j=1;j<MAXLENGTH-1;j++){
                Pheromone[i][j]=alpha*(Pheromone[i][j]-1)+1;
            }
    }

    void RemainPheromone(int* maze){
        while(!back_x.empty()&&!back_y.empty()){
            int px=back_x.pop();int py=back_y.pop();
            //            Pheromone[px][py]+=PheRate*((2*MAXLENGTH-4)*MAXNUMBER-length);
            Pheromone[px][py]+=PheRate*(MAXWAY-length);
        }
    }
    int Length(){return length;}
    void FindWay(int * maze){
        if (x==MAXLENGTH-2&&y==MAXLENGTH-2&&back==0){ back=1;back_x.pop();back_y.pop();}
        if (back==1){
            return;
        }
        else if (back==0){
            int direction;
            direction=SelectDirection(Pheromone[x][y+1],Pheromone[x+1][y]);
            if (direction==RIGHT&&y==MAXLENGTH-2) direction=DOWN;
            else if (direction==DOWN&&x==MAXLENGTH-2) direction=RIGHT;
            switch (direction) {
            case RIGHT:y++; break;
            case DOWN:x++; break;
            }
            length+=maze[x*MAXLENGTH+y];
            back_x.push(x);back_y.push(y);
        }
    }

};
