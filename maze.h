#ifndef MAZE_H
#define MAZE_H
#include<QTime>
#include<qqueue.h>
#include"windows.h"
const int MAXLENGTH = 19;//地图大小
static const int MAXNUMBER=20;//每格的最大值
static const int ROUTE=0;//默认值
static const int WALL=MAXNUMBER+1;//墙
static const int LEFT=-1,RIGHT=-2,UP=-3,DOWN=-4;//上下左右常量
const int INF=0xFFFFFF;//默认最大值
static bool DPPath[MAXLENGTH][MAXLENGTH];//保存DP算法求得的答案

class MyMaze{
private:
    int maze[MAXLENGTH][MAXLENGTH];//地图
public:
    MyMaze(){//初始化地图
        for (int i=0;i<MAXLENGTH;i++)
            for(int j=0;j<MAXLENGTH;j++){
                DPPath[i][j]=0;
                maze[i][j]=ROUTE;
            }
    }
    void MazeToAry(int* s) {//将二维数组转成一维数组
        for (int i=0;i<MAXLENGTH;i++)
            for (int j=0;j<MAXLENGTH;j++)
                s[i*MAXLENGTH+j]=maze[i][j];
    }

    void CreateMazeWithWeights(){//随机生成带值图
        qsrand((uint)QTime::currentTime().msec());
        for (int i=1;i<MAXLENGTH-1;i++)
            for (int j=1;j<MAXLENGTH-1;j++){
                maze[i][j]=qrand()%MAXNUMBER + 1;
            }
        maze[1][1]=ROUTE;maze[MAXLENGTH-2][MAXLENGTH-2]=ROUTE;//起终点设置
        for (int i=0;i<MAXLENGTH;i++){//周围一圈墙
            maze[i][MAXLENGTH-1]=WALL;
            maze[MAXLENGTH-1][i]=WALL;
            maze[0][i]=WALL;
            maze[i][0]=WALL;
        }
    }
    int DP(){//动态规划算法
        int TriangleMaze[2*MAXLENGTH-1][MAXLENGTH];
        //初始化数组
        for (int i=0;i<2*MAXLENGTH-1;i++)
            for (int j=0;j<MAXLENGTH;j++)
                TriangleMaze[i][j]=INF;
        for (int i=1;i<MAXLENGTH-1;i++){
            for (int j=1;j<MAXLENGTH-1;j++){
                TriangleMaze[i+j][j]=maze[i][j];
            }
        }
        //动态规划
        for (int i=2*MAXLENGTH-5;i>1;i--){
            for (int j=1;j<i;j++)
                if (TriangleMaze[i][j]!=INF){//&&(TriangleMaze[i+1][j]!=INF||TriangleMaze[i+1][j+1]!=INF)
                    if (TriangleMaze[i+1][j]>TriangleMaze[i+1][j+1])
                        TriangleMaze[i][j]+=TriangleMaze[i+1][j+1];
                    else TriangleMaze[i][j]+=TriangleMaze[i+1][j];
                }
        }
        //还原解
        int j=1;
        for (int i=2;i<2*MAXLENGTH-3;i++){
            DPPath[i-j][j]=1;//保存最优解的路径
            if (TriangleMaze[i+1][j]>TriangleMaze[i+1][j+1]) j++;
        }
        DPPath[1][1]=0;
        DPPath[MAXLENGTH-2][MAXLENGTH-2]=0;
        int sum=0;
        for(int i=1;i<MAXLENGTH-1;i++)
            for (int j=1;j<MAXLENGTH-1;j++)
                if (DPPath[i][j])
                    sum+=maze[i][j];
        return TriangleMaze[2][1];//返回最优解
    }
};
#endif // MAZE_H
